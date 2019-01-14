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
    m_player->setPhysicsSize(QSize(2,2));
    m_player->setPhysicsPosition(QPointF(0,0));
    m_player->setSize(QSize(2,2));
    m_player->setCategoryFlag(GameObject::PhysicsCharacter);
    m_player->setCollisionFlag(static_cast<GameObject::PhysicsFlags>(491));
    m_player->setBodyType(GameObject::BodyTypeDynamic);
    m_player->inventory()->setAutoParent(true);

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
    m_gameItems->setAutoParent(true);

    m_enemyItems = new GameItems(this);
    m_enemyItems->setAutoParent(true);

    m_characterItems = new GameItems(this);
    m_characterItems->setAutoParent(true);

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

GameItems *GameWorld::enemyItems() const
{
    return m_enemyItems;
}

PlayerController *GameWorld::playerController() const
{
    return m_playerController;
}

GameItem *GameWorld::playerFocusItem() const
{
    return m_playerFocusItem;
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

void GameWorld::pauseGame()
{
    setState(StatePaused);
}

void GameWorld::resumeGame()
{
    setState(StateRunning);
}

void GameWorld::inventoryClicked()
{
    onInventoryClicked();
}

void GameWorld::unlockLeftClicked()
{
    if (!m_currentChestItem)
        return;

    onLeftClicked();
}

void GameWorld::unlockRightClicked()
{
    if (!m_currentChestItem)
        return;

    onRightClicked();
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
    qCDebug(dcWorld()) << "Give up unlocking.";
    setCurrentChestItem(nullptr);
    setCurrentPlunderItems(nullptr);
    setState(StateRunning);
}

void GameWorld::finishPlunder()
{
    qCDebug(dcWorld()) << "Finish plundering";
    setCurrentChestItem(nullptr);
    setCurrentPlunderItems(nullptr);
    setState(StateRunning);
}

void GameWorld::performHitAttack(Character *attacker, Character *victim, int damage)
{
    // TODO: get sword damage
    qCDebug(dcWorld()) << attacker << "HITTING" << victim;
    victim->setHealth(victim->health() - damage);
}

void GameWorld::performShootImpact(Character *attacker, Character *victim, int damage)
{
    qCDebug(dcWorld()) << attacker << "SHOT" << victim << damage;
    victim->setHealth(victim->health() - damage);
}

void GameWorld::performBurnDamage(Character *victim, int damage)
{
    qCDebug(dcWorld()) << "Burn damage" << victim << damage;
    victim->setHealth(victim->health() - damage);
}

void GameWorld::takeItem(GameItems *gameItems, GameItem *item)
{
    qCDebug(dcWorld()) << "Move" << item << "to inventory";

    // Get the list of all matching items
    QList<GameItem *> targetItems;
    foreach (GameItem * targetItem, gameItems->gameItems()) {
        if (targetItem->itemId() == item->itemId()) {
            targetItems.append(targetItem);
        }
    }

    // If there is no left
    if (targetItems.count() == 0)
        return;

    // If the item is the last one
    if (targetItems.count() == 1) {
        gameItems->removeGameItem(targetItems.first());
        m_player->inventory()->addGameItem(targetItems.first());
    }

    // Take one which is not the current selected item
    foreach (GameItem * targetItem, targetItems) {
        if (targetItem->itemId() == item->itemId() && targetItem != item) {
            gameItems->removeGameItem(targetItem);
            m_player->inventory()->addGameItem(targetItem);
            return;
        }
    }
}

void GameWorld::takeAllItems(GameItems *gameItems)
{
    foreach (GameItem *item, gameItems->gameItems()) {
        gameItems->removeGameItem(item);
        m_player->inventory()->addGameItem(item);
    }
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
    case StatePlunder:
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
    emit playerFocusItemChanged(m_playerFocusItem);
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
        setCurrentPlunderItems(m_currentChestItem->items());
        if (m_currentChestItem->locked()) {
            qCDebug(dcWorld()) << "The chest is locked. Show unlock screen";
            setState(StateUnlocking);
        } else {
            qCDebug(dcWorld()) << "The chest is not locked. Show plunder screen";
            setState(StatePlunder);
        }
    } else {
        qCDebug(dcWorld()) << "Reset chest item.";
        setCurrentPlunderItems(nullptr);
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
}

Field *GameWorld::getFieldFromPosition(const QPointF position) const
{
    int x = static_cast<int>(position.x());
    int y = static_cast<int>(position.y());
    return m_map->getField(x, y);
}

void GameWorld::evaluatePlayerFocus()
{
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

        if (m_playerVisibleItems.first() == m_playerFocusItem)
            return;

        // Unset current focus item
        if (m_playerFocusItem) {
            m_playerFocusItem->setPlayerFocus(false);
        }

        setPlayerFocusItem(m_playerVisibleItems.first());
        m_playerVisibleItems.first()->setPlayerFocus(true);
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
        // Unset current focus item
        if (m_playerFocusItem) {
            m_playerFocusItem->setPlayerFocus(false);
        }

        closestItem.first->setPlayerFocus(true);
        setPlayerFocusItem(closestItem.first);
    }
}

void GameWorld::pickItem(GameItem *item)
{
    qCDebug(dcWorld()) << "Pick up item" << item;

    item->setPlayerFocus(false);
    item->setPlayerVisible(false);

    m_gameItems->removeGameItem(item);

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

    if (!m_loaded) {

        qCDebug(dcWorld()) << "Initialize visible items";

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
                connect(item, &GameItem::playerOnItemChanged, this, &GameWorld::onItemPlayerOnItemChanged);
                item->setParent(this);

                if (item->itemType() == GameItem::TypeCharacter) {
                    m_characterItems->addGameItem(item);
                } else if (item->itemType() == GameItem::TypeEnemy) {
                    m_enemyItems->addGameItem(item);
                    Enemy *enemy = qobject_cast<Enemy *>(item);
                    connect(enemy, &Enemy::killed, this, &GameWorld::onEnemyKilled);
                } else {
                    m_gameItems->addGameItem(item);
                }
            }
        }

        // Init stuff
        setBoundingSize(QSize(15, 15));
        m_currentViewOffset = QPoint(0, 0);

        m_player->setPosition(m_map->playerStartPosition());
        m_player->setName(Game::instance()->settings()->playerName());
        m_playerController->setControlMode(Game::instance()->settings()->controlMode());
    }

    setLoading(false);
    setLoaded(true);

    doPlayerMovement();
    evaluatePlayerFocus();
    setState(StateRunning);
}

void GameWorld::onItemPlayerVisibleChanged(bool playerVisible)
{
    GameItem *item = qobject_cast<GameItem *>(sender());

    // Add / remove item from visible items list
    if (playerVisible) {
        m_playerVisibleItems.append(item);
    } else {
        m_playerVisibleItems.removeAll(item);
    }

    evaluatePlayerFocus();
}

void GameWorld::onItemPlayerOnItemChanged(bool playerOnItem)
{
    GameItem *item = qobject_cast<GameItem *>(sender());
    if (playerOnItem) {
        switch (item->itemType()) {
        case GameItem::TypeEnemy: {
            Enemy *enemy = qobject_cast<Enemy *>(item);
            if (enemy->touchDamage()) {
                qCDebug(dcWorld()) << "!!!!!!!! Player touch damage" << enemy->touchDamage() << "from" << enemy;
                m_player->setHealth(m_player->health() - enemy->touchDamage());
            }
            break;
        }
        default:
            break;
        }
    }

}

void GameWorld::onEnemyKilled()
{
    GameItem *item = qobject_cast<GameItem *>(sender());
    if (m_playerFocusItem == item) {
        setPlayerFocusItem(nullptr);
    }

    if (m_playerVisibleItems.contains(item)) {
        m_playerVisibleItems.removeAll(item);
    }

    m_enemyItems->removeGameItem(item);
    item->deleteLater();

    evaluatePlayerFocus();
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
        case GameItem::TypeFirearm:
        case GameItem::TypeWeapon:
            if (m_playerFocusItem->interaction() == GameItem::InteractionPick) {
                m_playerFocusItem->performInteraction();
                pickItem(m_playerFocusItem);
                m_playerFocusItem = nullptr;
                evaluatePlayerFocus();
            }
            break;
        case GameItem::TypeChest:
            if (m_playerFocusItem->interaction() == GameItem::InteractionOpen) {
                ChestItem *chestItem = qobject_cast<ChestItem *>(m_playerFocusItem);
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
        case GameItem::TypeEnemy:
            if (m_playerFocusItem->interaction() == GameItem::InteractionAttack) {
                m_playerFocusItem->performInteraction();

                // Fixme: check sword and range
                qCDebug(dcWorld()) << "Player hitting";
                m_player->setHitNumber(m_player->hitNumber() + 1);
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
