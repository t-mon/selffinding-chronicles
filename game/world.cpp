#include "world.h"
#include "game.h"

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

QPointF World::adjustCollition(qreal dx, qreal dy)
{
    QPointF resultPosition = QPointF(m_player->position().x() + dx, m_player->position().y() + dy);
    qreal offset = m_player->size().width() / 2.0;

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
        qreal overlapX = checkField->position().x() - (resultPosition.x() + offset);
        qreal overlapY = checkField->position().y() - (resultPosition.y() + offset);

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x direction is smaller
                resultPosition = QPointF(checkField->position().x() - offset, resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() - offset);
            }
        }
    }

    // SouthWest field
    checkField = m_currentPlayerField->southWestField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = (resultPosition.x() - offset) - (checkField->position().x() + 1);
        qreal overlapY = checkField->position().y() - (resultPosition.y() + offset);

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x is smaller
                resultPosition = QPointF(checkField->position().x() + 1 + offset, resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                // Overlap in y is smaller
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() - offset);
            }
        }
    }

    // NorthEast field
    checkField = m_currentPlayerField->northEastField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = checkField->position().x() - (resultPosition.x() + offset);
        qreal overlapY = (resultPosition.y() - offset) - (checkField->position().y() + 1);

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x direction is smaller
                resultPosition = QPointF(checkField->position().x() - offset, resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() + 1 + offset);
            }
        }
    }

    // NorthWest field
    checkField = m_currentPlayerField->northWestField();
    if (checkField && !checkField->accessible()) {
        qreal overlapX = (resultPosition.x() - offset) - (checkField->position().x() + 1);
        qreal overlapY = (resultPosition.y() - offset) - (checkField->position().y() + 1);

        // If the player and the field overlap
        if (overlapX < 0 && overlapY < 0) {
            // Now check which overlap is bigger and reset the smaller one
            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
                // Overlap in x direction is smaller
                resultPosition = QPointF(checkField->position().x() + 1 + offset, resultPosition.y());
            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
                resultPosition = QPointF(resultPosition.x(), checkField->position().y() + 1 + offset);
            }
        }
    }

    return resultPosition;
}

void World::evaluateInRangeFields(const QPointF &playerPosition)
{
    // Check which fields are in the player aura
    Q_UNUSED(playerPosition)

    // Get surrounding fields and check if they are in range
    Field *currentField = getFieldOfPosition(playerPosition);
    QList<Field *> surroundingFields = currentField->getSurroundingFields();
    surroundingFields.append(currentField);

    // Remove fields which are not surrounded any more
    foreach (Field *field, m_fieldsInRange) {
        if (!surroundingFields.contains(field)) {
            field->setInPlayerRange(false);
            m_fieldsInRange.removeOne(field);
        }
    }

    foreach (Field *field, surroundingFields) {
        if (!m_fieldsInRange.contains(field)) {
            field->setInPlayerRange(true);
            m_fieldsInRange.append(field);
        }
    }

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
    evaluateInRangeFields(m_player->position() + delta);
    foreach (Field *field, m_fieldsInRange) {
        if (field->gameItem()) {
            if (m_collisionDetector->checkCollision(m_player, field->gameItem())) {
                qCDebug(dcWorld()) << "Player collision with" << field->gameItem();
            }
        }
    }

    // Collition detection
    QPointF resultPosition = adjustCollition(delta.x(), delta.y());

    // Calculate in range fields
    evaluateInRangeFields(resultPosition);

    // Finally set the position
    m_player->setPosition(resultPosition);
}

Field *World::getFieldOfPosition(const QPointF position) const
{
    int x = static_cast<int>(position.x());
    int y = static_cast<int>(position.y());
    return m_map->getField(x, y);
}

void World::onPlayerPositionChanged()
{
    // Calculate the current field on the map
    int x = static_cast<int>(m_player->position().x());
    int y = static_cast<int>(m_player->position().y());
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
        if (field->gameItem()) {
            m_gameItems->addItem(field->gameItem());
        }
    }

    // Init stuff
    setBoundingSize(QSize(15, 15));
    m_currentViewOffset = QPoint(0, 0);
    m_player->setPosition(m_map->playerStartPosition());
    doPlayerMovement();

    setLoading(false);
    setLoaded(true);

    // Start the game after loading
    Game::instance()->setRunning(true);
}

void World::tick()
{
    doPlayerMovement();

    // Do enemy movements

    // Evaluator bullets

    // Do other evaluation stuff
}
