#include "gameworld.h"
#include "game.h"

#include "gameobject.h"
#include "debugcategories.h"
#include "collisiondetector.h"

#include <QtMath>
#include <QtConcurrent/QtConcurrent>

GameWorld::GameWorld(QObject *parent) :
    Fields(parent),
    m_size(QSize(40, 40))
{
    // Create the player
    m_player = new Character(this);
    m_player->setShape(GameObject::ShapeCircle);
    m_player->setSize(QSize(2,2));
    connect(m_player, &Character::positionChanged, this, &GameWorld::onPlayerPositionChanged);

    // Create player controller
    m_playerController = new PlayerController(m_player, this);
    connect(m_playerController, &PlayerController::primaryActionPressedChanged, this, &GameWorld::onPrimaryActionPressedChanged);
    connect(m_playerController, &PlayerController::secondaryActionPressedChanged, this, &GameWorld::onSecondaryActionPressedChanged);
    connect(m_playerController, &PlayerController::leftClicked, this, &GameWorld::onLeftClicked);
    connect(m_playerController, &PlayerController::rightClicked, this, &GameWorld::onRightClicked);
    connect(m_playerController, &PlayerController::forwardClicked, this, &GameWorld::onForwardClicked);
    connect(m_playerController, &PlayerController::backwardClicked, this, &GameWorld::onBackwardClicked);
    connect(m_playerController, &PlayerController::inventoryPressed, this, &GameWorld::onInventoryClicked);

    m_collisionDetector = new CollisionDetector(this);

    m_gameItems = new GameItems(this);
    m_characterItems = new GameItems(this);

    // Create map and loading watcher
    m_map = new Map(this);
    m_loadingWatcher = new QFutureWatcher<void>(this);
    connect(m_loadingWatcher, &QFutureWatcher<void>::finished, this, &GameWorld::onLoadingFinished);
}

GameWorld::State GameWorld::state() const
{
    return m_state;
}

QSize GameWorld::size() const
{
    return m_size;
}

void GameWorld::setSize(const QSize &size)
{
    if (m_size == size)
        return;

    qCDebug(dcWorld()) << "Size changed" << size;
    m_size = size;
    emit sizeChanged(m_size);
}

QSize GameWorld::boundingSize() const
{
    return m_boundingSize;
}

void GameWorld::setBoundingSize(const QSize &boundingSize)
{
    if (m_boundingSize == boundingSize)
        return;

    qCDebug(dcWorld()) << "Bounding size changed" << boundingSize;
    m_boundingSize = boundingSize;
    emit boundingSizeChanged(m_boundingSize);
}

QPoint GameWorld::currentViewOffset() const
{
    return m_currentViewOffset;
}

void GameWorld::setCurrentViewOffset(const QPoint &currentViewOffset)
{
    if (m_currentViewOffset == currentViewOffset)
        return;

    qCDebug(dcWorld()) << "Current view offset changed" << currentViewOffset;
    m_currentViewOffset = currentViewOffset;
    emit currentViewOffsetChanged(m_currentViewOffset);
}

Map *GameWorld::map() const
{
    return m_map;
}

Character *GameWorld::player() const
{
    return m_player;
}

GameItems *GameWorld::gameItems() const
{
    return m_gameItems;
}

GameItems *GameWorld::characterItems() const
{
    return m_characterItems;
}

PlayerController *GameWorld::playerController() const
{
    return m_playerController;
}

Conversation *GameWorld::currentConversation() const
{
    return m_currentConversation;
}

ChestItem *GameWorld::currentChestItem() const
{
    return m_currentChestItem;
}

GameItems *GameWorld::currentPlunderItems() const
{
    return m_currentPlunderItems;
}

bool GameWorld::loaded() const
{
    return m_loaded;
}

bool GameWorld::loading() const
{
    return m_loading;
}

void GameWorld::loadMap(const QString &fileName)
{
    setState(StateLoading);
    if (m_map->fileName() == fileName && m_loaded) {
        qCDebug(dcWorld()) << "Map" << fileName << "already loaded.";
        onLoadingFinished();
        return;
    }

    qCDebug(dcWorld()) << "Start loading map" << fileName;
    setLoaded(false);
    setLoading(true);
    m_loadingWatcher->setFuture(QtConcurrent::run(m_map, &Map::loadMap, fileName));
}

void GameWorld::giveUpUnlocking()
{
    if (m_currentChestItem) {
        qCDebug(dcWorld()) << "Give up unlocking.";
        setCurrentChestItem(nullptr);
        setState(StateRunning);
    }
}

void GameWorld::finishPlunder()
{
    setCurrentPlunderItems(nullptr);
    setState(StateRunning);
}

void GameWorld::setState(GameWorld::State state)
{
    if (m_state == state)
        return;

    qCDebug(dcWorld()) << "--> State changed:" << state;
    m_state = state;
    emit stateChanged(m_state);

    switch (state) {
    case StateLoading:
        break;
    case StateRunning:
        m_player->setMovable(true);
        Game::instance()->setRunning(true);
        break;
    case StatePaused:
        m_player->setMovable(false);
        Game::instance()->setRunning(false);
        break;
    case StateInventory:
        m_player->setMovable(false);
        Game::instance()->setRunning(false);
        break;
    case StateUnlocking:
        m_player->setMovable(false);
        Game::instance()->setRunning(false);
        break;
    case StateConversation:
        m_player->setMovable(false);
        break;
    default:
        break;
    }

}

QPoint GameWorld::currentPlayerPosition() const
{
    return m_currentPlayerPosition;
}

Field *GameWorld::currentPlayerField() const
{
    return m_currentPlayerField;
}

void GameWorld::setCurrentPlayerPosition(const QPoint &currentPosition)
{
    if (m_currentPlayerPosition == currentPosition)
        return;

    qCDebug(dcWorld()) << "Current player position changed" << currentPosition;
    m_currentPlayerPosition = currentPosition;
    emit currentPlayerPositionChanged(m_currentPlayerPosition);
}

void GameWorld::setCurrentPlayerField(Field *field)
{
    if (m_currentPlayerField == field)
        return;

    // Update old field
    if (m_currentPlayerField)
        m_currentPlayerField->setPlayerOnField(false);

    m_currentPlayerField = field;

    if (m_currentPlayerField)
        m_currentPlayerField->setPlayerOnField(true);

//    // Set the property if a player is currently standing on an item
//    if (m_currentPlayerField->hasItem()) {
//        GameItem *item = m_currentPlayerField->gameItems()->gameItems().last();
//        item->setPlayerOnItem(true);
//        m_playerCurrentlyOnItem = item;
//    } else {
//        if (m_playerCurrentlyOnItem) {
//            m_playerCurrentlyOnItem->setPlayerOnItem(false);
//            m_playerCurrentlyOnItem = nullptr;
//        }
//    }

    emit currentPlayerFieldChanged(m_currentPlayerField);
}

void GameWorld::setPlayerFocusItem(GameItem *focusItem)
{
    if (m_playerFocusItem == focusItem)
        return;

    if (focusItem) {
        qCDebug(dcWorld()) << "Player focus item" << focusItem;
    } else {
        qCDebug(dcWorld()) << "Player has no item in focus";
    }

    m_playerFocusItem = focusItem;
}

void GameWorld::setLoaded(bool loaded)
{
    if (m_loaded == loaded)
        return;

    qCDebug(dcWorld()) << "Loaded changed:" << loaded;
    m_loaded = loaded;
    emit loadedChanged(m_loaded);
}

void GameWorld::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    qCDebug(dcWorld()) << "Loading changed:" << loading;
    m_loading = loading;
    emit loadingChanged(m_loading);
}

void GameWorld::setCurrentConversation(Conversation *conversation)
{
    if (m_currentConversation == conversation)
        return;

    m_currentConversation = conversation;
    emit currentConversationChanged(m_currentConversation);

    if (m_currentConversation) {
        m_player->setMovable(false);
        setState(StateConversation);
    } else {
        m_player->setMovable(true);
        setState(StateRunning);
    }
}

void GameWorld::setCurrentChestItem(ChestItem *chestItem)
{
    if (m_currentChestItem == chestItem)
        return;

    m_currentChestItem = chestItem;
    emit currentChestItemChanged(m_currentChestItem);

    if (m_currentChestItem) {
        m_player->setMovable(false);
        if (m_currentChestItem->locked()) {
            setState(StateUnlocking);
        } else {
            m_currentPlunderItems = m_currentChestItem->items();
            setState(StatePlunder);
        }
    } else {
        m_player->setMovable(true);
    }
}

void GameWorld::setCurrentPlunderItems(GameItems *plunderItems)
{
    if (m_currentPlunderItems == plunderItems)
        return;

    m_currentPlunderItems = plunderItems;
    emit currentPlunderItemsChanged(m_currentPlunderItems);
}

void GameWorld::doPlayerMovement()
{
    if (!m_player->movable())
        return;

    // FIXME: If primary pressed, default enable running for now
    m_player->setRunning(m_playerController->secondaryActionPressed());

//    QPointF delta = m_playerController->delta();

//    // If no movement
//    if (delta.isNull())
//        return;

//    // Check collision with object
//    evaluateInRangeFields(m_player->centerPosition() + delta);
//    foreach (Field *field, m_fieldsInRange) {
//        if (field->hasItem()) {
//            GameItem *item = field->gameItems()->gameItems().last();
//            if (m_collisionDetector->checkCollision(m_player, item)) {
//                //qCDebug(dcWorld()) << "Player behind" << field->gameItems()->gameItems().last();
//                item->setHidingPlayer(true);
//            } else {
//                item->setHidingPlayer(false);
//            }
//        }
//    }

//    // Collition detection
//    QPointF resultPosition = adjustCollition(delta);

//    // Finally set the position
//    m_player->setPosition(resultPosition);

//    // Calculate in range fields
//    evaluateInRangeFields(m_player->centerPosition());
}

Field *GameWorld::getFieldFromPosition(const QPointF position) const
{
    int x = static_cast<int>(position.x());
    int y = static_cast<int>(position.y());
    return m_map->getField(x, y);
}

QPointF GameWorld::adjustCollition(QPointF delta)
{
    QPointF resultPosition = m_player->position() + delta;

//    // Check border of the GameWorld

//    // North border
//    if (resultPosition.y() <= 0)
//        resultPosition = QPointF(resultPosition.x(), 0);

//    // East border
//    if (resultPosition.x() + m_player->size().width() >= m_size.width())
//        resultPosition = QPointF(m_size.width() - m_player->size().width(), resultPosition.y());

//    // South border
//    if (resultPosition.y() + m_player->size().height() >= m_size.height())
//        resultPosition = QPointF(resultPosition.x(), m_size.height() - m_player->size().height());

//    // West border
//    if (resultPosition.x() < 0)
//        resultPosition = QPointF(0, resultPosition.y());

//    if (!m_currentPlayerField)
//        return resultPosition;


//    // Check collision foreach surounding field
//    // North field
//    Field *checkField = m_currentPlayerField->northField();
//    if (checkField && !checkField->accessible() &&
//            resultPosition.y() < checkField->position().y() + m_player->size().height()) {
//        resultPosition = QPointF(resultPosition.x(), checkField->position().y() + m_player->size().height());
//    }

//    // South field
//    checkField = m_currentPlayerField->southField();
//    if (checkField && !checkField->accessible() &&
//            resultPosition.y() + m_player->size().height() >= checkField->position().y()) {
//        resultPosition = QPointF(resultPosition.x(), checkField->position().y() - m_player->size().height());
//    }

//    // East field
//    checkField = m_currentPlayerField->eastField();
//    if (checkField && !checkField->accessible() &&
//            resultPosition.x() + m_player->size().width() > checkField->position().x()) {
//        resultPosition = QPointF(checkField->position().x() - m_player->size().width(), resultPosition.y());
//    }

//    // West field
//    checkField = m_currentPlayerField->westField();
//    if (checkField && !checkField->accessible() &&
//            resultPosition.x() < checkField->position().x() + m_player->size().width()) {
//        resultPosition = QPointF(checkField->position().x() + m_player->size().width(), resultPosition.y());
//    }

//    // SouthEast field
//    checkField = m_currentPlayerField->southEastField();
//    if (checkField && !checkField->accessible()) {
//        qreal overlapX = checkField->position().x() - (resultPosition.x() + m_player->size().width());
//        qreal overlapY = checkField->position().y() - (resultPosition.y() + m_player->size().height());

//        // If the player and the field overlap
//        if (overlapX < 0 && overlapY < 0) {
//            // Now check which overlap is bigger and reset the smaller one
//            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
//                // Overlap in x direction is smaller
//                resultPosition = QPointF(checkField->position().x() - m_player->size().width(), resultPosition.y());
//            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
//                resultPosition = QPointF(resultPosition.x(), checkField->position().y() - m_player->size().height());
//            }
//        }
//    }

//    // SouthWest field
//    checkField = m_currentPlayerField->southWestField();
//    if (checkField && !checkField->accessible()) {
//        qreal overlapX = resultPosition.x() - (checkField->position().x() + 1);
//        qreal overlapY = checkField->position().y() - (resultPosition.y() +  m_player->size().height());

//        // If the player and the field overlap
//        if (overlapX < 0 && overlapY < 0) {
//            // Now check which overlap is bigger and reset the smaller one
//            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
//                // Overlap in x is smaller
//                resultPosition = QPointF(checkField->position().x() + 1, resultPosition.y());
//            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
//                // Overlap in y is smaller
//                resultPosition = QPointF(resultPosition.x(), checkField->position().y() - m_player->size().height());
//            }
//        }
//    }

//    // NorthEast field
//    checkField = m_currentPlayerField->northEastField();
//    if (checkField && !checkField->accessible()) {
//        qreal overlapX = checkField->position().x() - (resultPosition.x() + m_player->size().width());
//        qreal overlapY = resultPosition.y() - (checkField->position().y() + 1);

//        // If the player and the field overlap
//        if (overlapX < 0 && overlapY < 0) {
//            // Now check which overlap is bigger and reset the smaller one
//            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
//                // Overlap in x direction is smaller
//                resultPosition = QPointF(checkField->position().x() - m_player->size().width(), resultPosition.y());
//            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
//                resultPosition = QPointF(resultPosition.x(), checkField->position().y() + 1);
//            }
//        }
//    }

//    // NorthWest field
//    checkField = m_currentPlayerField->northWestField();
//    if (checkField && !checkField->accessible()) {
//        qreal overlapX = (resultPosition.x()) - (checkField->position().x() + 1);
//        qreal overlapY = (resultPosition.y()) - (checkField->position().y() + 1);

//        // If the player and the field overlap
//        if (overlapX < 0 && overlapY < 0) {
//            // Now check which overlap is bigger and reset the smaller one
//            if (qAbs(overlapX) < qAbs(overlapY) && qAbs(overlapY) < 0.5) {
//                // Overlap in x direction is smaller
//                resultPosition = QPointF(checkField->position().x() + 1, resultPosition.y());
//            } else if (qAbs(overlapX) > qAbs(overlapY) && qAbs(overlapX) < 0.5) {
//                resultPosition = QPointF(resultPosition.x(), checkField->position().y() + 1);
//            }
//        }
//    }

    return resultPosition;
}

void GameWorld::evaluateInRangeFields(const QPointF &playerPosition)
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

//    // Get fields in aura range
//    QList<Field *> fieldsInRange;
//    foreach (Field *field, fieldsToCheck) {
//        // Check fields in range
//        if (m_collisionDetector->checkCollision(m_player->auraCircleObject(), field->collitionObject())) {
//            fieldsInRange.append(field);
//        }
//    }

//    // Get player focus item
//    QHash<qreal, GameItem *> visibleItems;
//    foreach (Field *field, fieldsToCheck) {
//        // Check fields in range
//        if (field->hasItem()) {
//            GameItem *item = field->gameItems()->gameItems().first();
//            if (item->interaction() == GameItem::InteractionNone)
//                continue;

//            if (m_collisionDetector->checkCollision(m_player->auraCircleObject(), item)) {
//                visibleItems.insert(m_collisionDetector->calculateCenterDistance(m_player, item), item);
//            }
//        }
//    }

//    // Get the focus item
//    if (visibleItems.isEmpty()) {
//        // No item visible, reset also the old one
//        if (m_playerFocusItem) {
//            m_playerFocusItem->setPlayerFocus(false);
//            m_playerFocusItem = nullptr;
//        }
//    } else {
//        // Get the closest item
//        qreal closestDistance = 100000;
//        foreach (const qreal &distance, visibleItems.keys()) {
//            if (distance <= closestDistance) {
//                closestDistance = distance;
//            }
//        }

//        // Make sure it is not the same item
//        if (m_playerFocusItem) {
//            if (m_playerFocusItem != visibleItems.value(closestDistance)) {
//                m_playerFocusItem->setPlayerFocus(false);
//                m_playerFocusItem = nullptr;
//            }
//        }

//        m_playerFocusItem = visibleItems.value(closestDistance);
//        m_playerFocusItem->setPlayerFocus(true);
//    }

//    // Remove fields which are not surrounded any more
//    foreach (Field *field, m_fieldsInRange) {
//        if (!fieldsInRange.contains(field)) {
//            field->setInPlayerRange(false);
//            m_fieldsInRange.removeOne(field);
//        }
//    }

//    // Mark fields in range
//    foreach (Field *field, fieldsInRange) {
//        if (!m_fieldsInRange.contains(field)) {
//            field->setInPlayerRange(true);
//            m_fieldsInRange.append(field);
//        }
//    }
}

void GameWorld::pickItem(GameItem *item)
{
    qCDebug(dcWorld()) << "Pick up item" << item;

    item->setPlayerFocus(false);
    item->setPlayerVisible(false);

    m_gameItems->removeGameItem(item);

//    // Make fields accessable according to the unacessableMap
//    foreach(const QPoint &unaccessableOffset, item->unaccessableMap()) {
//        QPointF absolutCoordinate(item->position() + unaccessableOffset);
//        Field *field = m_map->getField(absolutCoordinate);
//        if (!field)
//            continue;

//        //field->setAccessible(true);
//    }

//    // Remove visible item parts from the map fields
//    foreach(const QPoint &visibilityOffset, item->visibilityMap()) {
//        QPointF absolutCoordinate(item->position() + visibilityOffset);
//        Field *field = m_map->getField(absolutCoordinate);
//        if (!field)
//            continue;

//        //field->gameItems()->removeGameItem(item);
//    }

    qCDebug(dcWorld()) << "Picked item and add it to the inventory" << item;
    m_player->inventory()->addGameItem(item);
}

void GameWorld::onPlayerPositionChanged()
{
    // Calculate the current field on the map
    int x = static_cast<int>(m_player->centerPosition().x());
    int y = static_cast<int>(m_player->centerPosition().y());
    setCurrentPlayerPosition(QPoint(x, y));
    setCurrentPlayerField(m_map->getField(x, y));
}

void GameWorld::onLoadingFinished()
{
    qCDebug(dcWorld()) << "Loading map finished";

    clearModel();

    qCDebug(dcWorld()) << "--> Initialize fields";
    for (int x = m_currentViewOffset.x(); x < m_currentViewOffset.x() + m_size.width(); x++) {
        for (int y = m_currentViewOffset.y(); y < m_currentViewOffset.y() + m_size.height(); y++) {
            Field *field = m_map->getField(x, y);
            field->setParent(this);
            addField(field);
        }
    }

    qCDebug(dcWorld()) << "--> Initialize items";
    foreach (Field *field, fields()) {
        foreach (GameItem *item, field->gameItems()->gameItems()) {
            connect(item, &GameItem::playerVisibleChanged, this, &GameWorld::onItemPlayerVisibleChanged);
            item->setParent(this);
            if (item->itemType() == GameItem::TypeCharacter) {
                m_characterItems->addGameItem(item);
            } else {
                m_gameItems->addGameItem(item);
            }
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
    setState(StateRunning);
}

void GameWorld::onItemPlayerVisibleChanged()
{
    GameItem *item = qobject_cast<GameItem *>(sender());
    // Add / remove item from visible items list
    if (item->playerVisible()) {
        m_playerVisibleItems.append(item);
    } else {
        m_playerVisibleItems.removeAll(item);
    }

    // If there is no visible item left
    if (m_playerVisibleItems.isEmpty()) {
        // If there was a focus item
        if (m_playerFocusItem) {
            m_playerFocusItem->setPlayerFocus(false);
        }

        setPlayerFocusItem(nullptr);
        return;
    }

    // If there is only one item visible
    if (m_playerVisibleItems.count() == 1) {

        if (item == m_playerFocusItem)
            return;

        // Unset current focus item
        if (m_playerFocusItem) {
            m_playerFocusItem->setPlayerFocus(false);
        }

        item->setPlayerFocus(true);
        setPlayerFocusItem(item);
        return;
    }

    // Multiple items visible, get the closest item
    QPair<GameItem *, double> closestItem;
    foreach (GameItem *visibleItem, m_playerVisibleItems) {
        if (!closestItem.first) {
            closestItem.first = visibleItem;
            closestItem.second = m_collisionDetector->calculateCenterDistance(m_player, visibleItem);
            continue;
        } else {
            double distance = m_collisionDetector->calculateCenterDistance(m_player, visibleItem);
            if (distance < closestItem.second) {
                closestItem.first = visibleItem;
                closestItem.second = distance;
            }
        }
    }

    if (closestItem.first) {
        closestItem.first->setPlayerFocus(true);
        setPlayerFocusItem(closestItem.first);
    }
}

void GameWorld::onPrimaryActionPressedChanged(bool pressed)
{
    qCDebug(dcWorld()) << "Primary action" << (pressed ? "pressed" : "released");

    switch (m_state) {
    case StateConversation:
        if (m_currentConversation && pressed) m_currentConversation->confirmPressed();
        return;
    default:
        break;
    }

    if (pressed && m_playerFocusItem && Game::instance()->running() && !m_currentConversation) {
        switch (m_playerFocusItem->itemType()) {
        case GameItem::TypePlant:
        case GameItem::TypeWeapon:
            if (m_playerFocusItem->interaction() == GameItem::InteractionPick) {
                m_playerFocusItem->performInteraction();
                pickItem(m_playerFocusItem);
                m_playerFocusItem = nullptr;
                evaluateInRangeFields(m_player->position());
            }
            break;
        case GameItem::TypeChest:
            if (m_playerFocusItem->interaction() == GameItem::InteractionOpen) {
                ChestItem *chestItem = qobject_cast<ChestItem *>(m_playerFocusItem);
                if (chestItem->locked()) {
                    qCDebug(dcWorld()) << "The chest is locked. Show unlock screen";
                } else {
                    qCDebug(dcWorld()) << "The chest is not locked. Show items screen";
                }
                setCurrentChestItem(chestItem);
            }
            break;
        case GameItem::TypeCharacter:
            if (m_playerFocusItem->interaction() == GameItem::InteractionTalk) {
                m_playerFocusItem->performInteraction();
                Character *character = qobject_cast<Character *>(m_playerFocusItem);

                // FIXME: load directly for now

                Conversation *conversation = new Conversation(character, DataLoader::loadJsonData(":/dialogs/test-dialog.json"));
                connect(conversation, &Conversation::conversationFinished, this, &GameWorld::onCurrentConversationFinished);

                if (m_currentConversation) {
                    m_currentConversation->deleteLater();
                    m_currentConversation = nullptr;
                }

                // Start conversation
                setCurrentConversation(conversation);
            }
            break;
        default:
            qCWarning(dcWorld()) << "Unhandled action on item" << m_playerFocusItem;
            break;
        }
        //qCDebug(dcWorld()) << "Perform interaction" << m_playerFocusItem->interaction();
        //m_playerFocusItem->performInteraction();
    }
}

void GameWorld::onSecondaryActionPressedChanged(bool pressed)
{
    switch (m_state) {
    case StateConversation:
        if (m_currentConversation && pressed) m_currentConversation->confirmPressed();
        break;
    default:
        qCDebug(dcWorld()) << "Secondary action" << (pressed ? "pressed" : "released");
        break;
    }
}

void GameWorld::onLeftClicked()
{
    switch (m_state) {
    case StateUnlocking:
        m_currentChestItem->unlockLeftMovement();
        if (!m_currentChestItem->locked()) {
            qCDebug(dcWorld()) << "Chest unlocked!";
            setState(StatePlunder);
        }
        break;
    default:
        break;
    }
}

void GameWorld::onRightClicked()
{
    switch (m_state) {
    case StateUnlocking:
        m_currentChestItem->unlockRightMovement();
        if (!m_currentChestItem->locked()) {
            qCDebug(dcWorld()) << "Chest unlocked!";
            setState(StatePlunder);
        }
        break;
    default:
        break;
    }
}

void GameWorld::onForwardClicked()
{
    switch (m_state) {
    case StateConversation:
        if (m_currentConversation) m_currentConversation->upPressed();
        break;
    default:
        break;
    }
}

void GameWorld::onBackwardClicked()
{
    switch (m_state) {
    case StateConversation:
        if (m_currentConversation) m_currentConversation->downPressed();
        break;
    default:
        break;
    }
}

void GameWorld::onInventoryClicked()
{
    qCDebug(dcWorld()) << "Inventory clicked";
    switch (m_state) {
    case StateRunning:
        setState(StateInventory);
        break;
    case StateInventory:
        setState(StateRunning);
        break;
    default:
        break;
    }
}

void GameWorld::onCurrentConversationFinished()
{
    qCDebug(dcWorld()) << "Conversation finished";
    m_currentConversation->deleteLater();
    setCurrentConversation(nullptr);
    setState(StateRunning);
}

void GameWorld::tick()
{
    doPlayerMovement();

    // Do enemy movements

    // Evaluator bullets

    // Do other evaluation stuff

    emit worldPostTick();
}
