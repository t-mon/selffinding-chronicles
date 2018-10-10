#include "world.h"
#include "game.h"

#include "gameobject.h"
#include "debugcategories.h"
#include "collisiondetector.h"

#include <QtMath>
#include <QtConcurrent/QtConcurrent>

World::World(QObject *parent) :
    Fields(parent),
    m_size(QSize(40, 40))
{
    // Create the player
    m_player = new Player(this);
    m_player->setShape(GameObject::ShapeCircle);
    connect(m_player, &Player::positionChanged, this, &World::onPlayerPositionChanged);

    // Create player controller
    m_playerController = new PlayerController(m_player, this);
    connect(m_playerController, &PlayerController::primaryActionPressedChanged, this, &World::onPrimaryActionPressedChanged);
    connect(m_playerController, &PlayerController::secondaryActionPressedChanged, this, &World::onSecundaryActionPressedChanged);

    m_collisionDetector = new CollisionDetector(this);

    m_gameItems = new GameItems(this);

    // Create map and loading watcher
    m_map = new Map(this);
    m_loadingWatcher = new QFutureWatcher<void>(this);
    connect(m_loadingWatcher, &QFutureWatcher<void>::finished, this, &World::onLoadingFinished);
}

QSize World::size() const
{
    return m_size;
}

void World::setSize(const QSize &size)
{
    if (m_size == size)
        return;

    qCDebug(dcWorld()) << "Size changed" << size;
    m_size = size;
    emit sizeChanged(m_size);
}

QSize World::boundingSize() const
{
    return m_boundingSize;
}

void World::setBoundingSize(const QSize &boundingSize)
{
    if (m_boundingSize == boundingSize)
        return;

    qCDebug(dcWorld()) << "Bounding size changed" << boundingSize;
    m_boundingSize = boundingSize;
    emit boundingSizeChanged(m_boundingSize);
}

QPoint World::currentViewOffset() const
{
    return m_currentViewOffset;
}

void World::setCurrentViewOffset(const QPoint &currentViewOffset)
{
    if (m_currentViewOffset == currentViewOffset)
        return;

    qCDebug(dcWorld()) << "Current view offset changed" << currentViewOffset;
    m_currentViewOffset = currentViewOffset;
    emit currentViewOffsetChanged(m_currentViewOffset);
}

Map *World::map()
{
    return m_map;
}

Player *World::player()
{
    return m_player;
}

GameItems *World::gameItems()
{
    return m_gameItems;
}

PlayerController *World::playerController()
{
    return m_playerController;
}

bool World::loaded() const
{
    return m_loaded;
}

bool World::loading() const
{
    return m_loading;
}

void World::loadMap(const QString &fileName)
{
    if (m_map->fileName() == fileName && m_loaded) {
        qCDebug(dcWorld()) << "Map" << fileName <<"already loaded.";
        onLoadingFinished();
        return;
    }

    qCDebug(dcWorld()) << "Start loading map" << fileName;
    setLoaded(false);
    setLoading(true);
    m_loadingWatcher->setFuture(QtConcurrent::run(m_map, &Map::loadMap, fileName));
}

QPoint World::currentPlayerPosition() const
{
    return m_currentPlayerPosition;
}

Field *World::currentPlayerField()
{
    return m_currentPlayerField;
}

void World::setCurrentPlayerPosition(const QPoint &currentPosition)
{
    if (m_currentPlayerPosition == currentPosition)
        return;

    qCDebug(dcWorld()) << "Current player position changed" << currentPosition;
    m_currentPlayerPosition = currentPosition;
    emit currentPlayerPositionChanged(m_currentPlayerPosition);
}

void World::setCurrentPlayerField(Field *field)
{
    if (m_currentPlayerField == field)
        return;

    // Update old field
    if (m_currentPlayerField)
        m_currentPlayerField->setPlayerOnField(false);

    m_currentPlayerField = field;

    if (m_currentPlayerField)
        m_currentPlayerField->setPlayerOnField(true);

    emit currentPlayerFieldChanged(m_currentPlayerField);
}

void World::setPlayerFocusItem(GameItem *focusItem)
{
    if (m_playerFocusItem == focusItem)
        return;

    m_playerFocusItem = focusItem;
}

void World::setLoaded(bool loaded)
{
    if (m_loaded == loaded)
        return;

    qCDebug(dcWorld()) << "Loaded changed:" << loaded;
    m_loaded = loaded;
    emit loadedChanged(m_loaded);
}

void World::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    qCDebug(dcWorld()) << "Loading changed:" << loading;
    m_loading = loading;
    emit loadingChanged(m_loading);
}

void World::doPlayerMovement()
{
    if (!m_player->movable())
        return;

    // FIXME: If primary pressed, default enable running for now
    m_player->setRunning(m_playerController->primaryActionPressed());

    QPointF delta = m_playerController->delta();

    // If no movement
    if (delta.isNull())
        return;

    // Check collision with object
    evaluateInRangeFields(m_player->centerPosition() + delta);
    foreach (Field *field, m_fieldsInRange) {
        if (field->hasItem() && field->gameItems()->gameItems().last()->interaction() != GameItem::InteractionNone) {
            if (m_collisionDetector->checkCollision(m_player, field->gameItems()->gameItems().last())) {
                qCDebug(dcWorld()) << "Player collision with" << field->gameItems()->gameItems().last();
            }
        }
    }

    // Collition detection
    QPointF resultPosition = adjustCollition(delta);

    // Finally set the position
    m_player->setPosition(resultPosition);

    // Calculate in range fields
    evaluateInRangeFields(m_player->centerPosition());
}

Field *World::getFieldFromPosition(const QPointF position) const
{
    int x = static_cast<int>(position.x());
    int y = static_cast<int>(position.y());
    return m_map->getField(x, y);
}

QPointF World::adjustCollition(QPointF delta)
{
    QPointF resultPosition = m_player->position() + delta;

    // Check border of the world

    // North border
    if (resultPosition.y() <= 0)
        resultPosition = QPointF(resultPosition.x(), 0);

    // East border
    if (resultPosition.x() + m_player->size().width() >= m_size.width())
        resultPosition = QPointF(m_size.width() - m_player->size().width(), resultPosition.y());

    // South border
    if (resultPosition.y() + m_player->size().height() >= m_size.height())
        resultPosition = QPointF(resultPosition.x(), m_size.height() - m_player->size().height());

    // West border
    if (resultPosition.x() < 0)
        resultPosition = QPointF(0, resultPosition.y());

    if (!m_currentPlayerField)
        return resultPosition;


    // Check collision foreach surounding field
    // North field
    Field *checkField = m_currentPlayerField->northField();
    if (checkField && !checkField->accessible() &&
            resultPosition.y() < checkField->position().y() + m_player->size().height()) {
        resultPosition = QPointF(resultPosition.x(), checkField->position().y() + m_player->size().height());
    }

    // South field
    checkField = m_currentPlayerField->southField();
    if (checkField && !checkField->accessible() &&
            resultPosition.y() + m_player->size().height() >= checkField->position().y()) {
        resultPosition = QPointF(resultPosition.x(), checkField->position().y() - m_player->size().height());
    }

    // East field
    checkField = m_currentPlayerField->eastField();
    if (checkField && !checkField->accessible() &&
            resultPosition.x() + m_player->size().width() > checkField->position().x()) {
        resultPosition = QPointF(checkField->position().x() - m_player->size().width(), resultPosition.y());
    }

    // West field
    checkField = m_currentPlayerField->westField();
    if (checkField && !checkField->accessible() &&
            resultPosition.x() < checkField->position().x() + m_player->size().width()) {
        resultPosition = QPointF(checkField->position().x() + m_player->size().width(), resultPosition.y());
    }

    // SouthEast field
    checkField = m_currentPlayerField->southEastField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = checkField->position().x() - (resultPosition.x() + m_player->size().width());
        qreal overlapY = checkField->position().y() - (resultPosition.y() + m_player->size().height());

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x direction is smaller
                resultPosition = QPointF(checkField->position().x() - m_player->size().width(), resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() - m_player->size().height());
            }
        }
    }

    // SouthWest field
    checkField = m_currentPlayerField->southWestField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = resultPosition.x() - (checkField->position().x() + 1);
        qreal overlapY = checkField->position().y() - (resultPosition.y() +  m_player->size().height());

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x is smaller
                resultPosition = QPointF(checkField->position().x() + 1, resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                // Overlap in y is smaller
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() - m_player->size().height());
            }
        }
    }

    // NorthEast field
    checkField = m_currentPlayerField->northEastField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = checkField->position().x() - (resultPosition.x() + m_player->size().width());
        qreal overlapY = resultPosition.y() - (checkField->position().y() + 1);

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x direction is smaller
                resultPosition = QPointF(checkField->position().x() - m_player->size().width(), resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() + 1);
            }
        }
    }

    // NorthWest field
    checkField = m_currentPlayerField->northWestField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = (resultPosition.x()) - (checkField->position().x() + 1);
        qreal overlapY = (resultPosition.y()) - (checkField->position().y() + 1);

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x direction is smaller
                resultPosition = QPointF(checkField->position().x() + 1, resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() + 1);
            }
        }
    }

    return resultPosition;
}

void World::evaluateInRangeFields(const QPointF &playerPosition)
{
    Field *currentField = getFieldFromPosition(playerPosition);
    QList<Field *> fieldsToCheck;
    Field *fieldIterator = nullptr;
    int iteratorCount = 0;

    // Get most north field
    iteratorCount = 0; fieldIterator = currentField;
    while (fieldIterator->northField() && iteratorCount < m_player->auraRange() + 1) {
        fieldsToCheck.append(fieldIterator);
        fieldIterator = fieldIterator->northField();
        iteratorCount++;
    }
    Field *mostNorthField = fieldIterator;

    // Get most south field
    iteratorCount = 0; fieldIterator = currentField;
    while (fieldIterator->southField() && iteratorCount < m_player->auraRange() + 1) {
        fieldsToCheck.append(fieldIterator);
        fieldIterator = fieldIterator->southField();
        iteratorCount++;
    }
    Field *mostSouthField = fieldIterator;

    // Get most west field
    iteratorCount = 0; fieldIterator = currentField;
    while (fieldIterator->westField() && iteratorCount < m_player->auraRange() + 1) {
        fieldsToCheck.append(fieldIterator);
        fieldIterator = fieldIterator->westField();
        iteratorCount++;
    }
    Field *mostWestField = fieldIterator;

    // Get most east field
    iteratorCount = 0; fieldIterator = currentField;
    while (fieldIterator->eastField() && iteratorCount < m_player->auraRange() + 1) {
        fieldsToCheck.append(fieldIterator);
        fieldIterator = fieldIterator->eastField();
        iteratorCount++;
    }
    Field *mostEastField = fieldIterator;

    // Now add all these fields to the check list
    for (int x = mostWestField->position().x(); x <= mostEastField->position().x(); x++) {
        for (int y = mostNorthField->position().y(); y <= mostSouthField->position().y(); y++) {
            Field *field = getFieldFromPosition(QPointF(x, y));
            if (!fieldsToCheck.contains(field)) {
                fieldsToCheck.append(field);
            }
        }
    }

    // Get fields in aura range
    QList<Field *> fieldsInRange;
    foreach (Field *field, fieldsToCheck) {
        // Check fields in range
        if (m_collisionDetector->checkCollision(m_player->auraCircleObject(), field->collitionObject())) {
            fieldsInRange.append(field);
        }
    }

    // Get player focus item
    QHash<qreal, GameItem *> visibleItems;
    foreach (Field *field, fieldsToCheck) {
        // Check fields in range
        if (field->hasItem()) {
            GameItem *item = field->gameItems()->gameItems().first();
            if (item->interaction() == GameItem::InteractionNone)
                continue;

            if (m_collisionDetector->checkCollision(m_player->auraCircleObject(), item)) {
                visibleItems.insert(m_collisionDetector->calculateCenterDistance(m_player, item), item);
            }
        }
    }

    // Get the focus item
    if (visibleItems.isEmpty()) {
        // No item visible, reset also the old one
        if (m_playerFocusItem) {
            m_playerFocusItem->setPlayerFocus(false);
            m_playerFocusItem = nullptr;
        }
    } else {
        // Get the closest item
        qreal closestDistance = 100000;
        foreach (const qreal &distance, visibleItems.keys()) {
            if (distance <= closestDistance) {
                closestDistance = distance;
            }
        }

        // Make sure it is not the same item
        if (m_playerFocusItem) {
            if (m_playerFocusItem != visibleItems.value(closestDistance)) {
                m_playerFocusItem->setPlayerFocus(false);
                m_playerFocusItem = nullptr;
            }
        }

        m_playerFocusItem = visibleItems.value(closestDistance);
        m_playerFocusItem->setPlayerFocus(true);
    }

    // Remove fields which are not surrounded any more
    foreach (Field *field, m_fieldsInRange) {
        if (!fieldsInRange.contains(field)) {
            field->setInPlayerRange(false);
            m_fieldsInRange.removeOne(field);
        }
    }

    // Mark fields in range
    foreach (Field *field, fieldsInRange) {
        if (!m_fieldsInRange.contains(field)) {
            field->setInPlayerRange(true);
            m_fieldsInRange.append(field);
        }
    }
}

void World::pickItem(GameItem *item)
{
    qCDebug(dcWorld()) << "Pick up item" << item;

    // Make fields accessable according to the unacessableMap
    foreach(const QPoint &unaccessableOffset, item->unaccessableMap()) {
        QPointF absolutCoordinate(item->position() + unaccessableOffset);
        Field *field = m_map->getField(absolutCoordinate);
        if (!field)
            continue;

        field->setAccessible(true);
    }

    // Remove visible item parts from the map fields
    foreach(const QPoint &visibilityOffset, item->visibilityMap()) {
        QPointF absolutCoordinate(item->position() + visibilityOffset);
        Field *field = m_map->getField(absolutCoordinate);
        if (!field)
            continue;

        field->gameItems()->removeGameItem(item);
    }

    m_gameItems->removeGameItem(item);
    m_player->inventory()->addGameItem(item);
}

void World::onPlayerPositionChanged()
{
    // Calculate the current field on the map
    int x = static_cast<int>(m_player->centerPosition().x());
    int y = static_cast<int>(m_player->centerPosition().y());
    setCurrentPlayerPosition(QPoint(x, y));
    setCurrentPlayerField(m_map->getField(x, y));
}

void World::onLoadingFinished()
{
    qCDebug(dcWorld()) << "Loading map finished";

    clearModel();

    qCDebug(dcWorld()) << "--> Initialize fields";
    for (int x = m_currentViewOffset.x(); x < m_currentViewOffset.x() + m_size.width(); x++) {
        for (int y = m_currentViewOffset.y(); y < m_currentViewOffset.y() + m_size.height(); y++) {
            Field *field = m_map->getField(x, y);
            addField(field);
        }
    }

    qCDebug(dcWorld()) << "--> Initialize items";
    foreach (Field *field, fields()) {
        foreach (GameItem *item, field->gameItems()->gameItems()) {
            m_gameItems->addGameItem(item);
        }
    }

    // Init stuff
    setBoundingSize(QSize(15, 15));
    m_currentViewOffset = QPoint(0, 0);
    m_player->setPosition(m_map->playerStartPosition());
    evaluateInRangeFields(m_player->position());
    doPlayerMovement();

    setLoading(false);
    setLoaded(true);

    // Start the game after loading
    Game::instance()->setRunning(true);
}

void World::onPrimaryActionPressedChanged(bool pressed)
{
    qCDebug(dcWorld()) << "Primary action" << (pressed ? "pressed" : "released");
}

void World::onSecundaryActionPressedChanged(bool pressed)
{
    qCDebug(dcWorld()) << "Secundary action" << (pressed ? "pressed" : "released");
    if (m_playerFocusItem && pressed) {
        switch (m_playerFocusItem->itemType()) {
        case GameItem::TypePlant:
            if (m_playerFocusItem->interaction() == GameItem::InteractionPick) {
                pickItem(m_playerFocusItem);
                m_playerFocusItem = nullptr;
                evaluateInRangeFields(m_player->position());
            }
            break;
        default:
            break;
        }

        //qCDebug(dcWorld()) << "Perform interaction" << m_playerFocusItem->interaction();
        //m_playerFocusItem->performInteraction();
    }


}

void World::tick()
{
    doPlayerMovement();

    // Do enemy movements

    // Evaluator bullets

    // Do other evaluation stuff
}
