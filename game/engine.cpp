#include "engine.h"
#include "game.h"

#include "gameobject.h"
#include "debugcategories.h"
#include "collisiondetector.h"

#include <QtMath>
#include <QtConcurrent/QtConcurrent>

Engine::Engine(QObject *parent) :
    QObject(parent)
{
    m_dataManager = new DataManager(this);
    connect(m_dataManager, &DataManager::stateChanged, this, &Engine::onDataManagerStateChanged);

    m_activeItems = new GameItemsProxy(this);
    m_activeItems->setGameItems(m_dataManager->items());

    m_activeEnemies = new GameItemsProxy(this);
    m_activeEnemies->setGameItems(m_dataManager->enemies());

    m_activeCharacters = new GameItemsProxy(this);
    m_activeCharacters->setGameItems(m_dataManager->characters());

    connect(m_activeItems, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);
    connect(m_activeEnemies, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);
    connect(m_activeCharacters, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);

    setViewWindow(QRectF(0, 0, 10, 10));

    // Create player controller
    m_playerController = new PlayerController(this);
    connect(m_playerController, &PlayerController::primaryActionPressedChanged, this, &Engine::onPrimaryActionPressedChanged);
    connect(m_playerController, &PlayerController::secondaryActionPressedChanged, this, &Engine::onSecondaryActionPressedChanged);
    connect(m_playerController, &PlayerController::leftClicked, this, &Engine::onLeftClicked);
    connect(m_playerController, &PlayerController::rightClicked, this, &Engine::onRightClicked);
    connect(m_playerController, &PlayerController::forwardClicked, this, &Engine::onForwardClicked);
    connect(m_playerController, &PlayerController::backwardClicked, this, &Engine::onBackwardClicked);
    connect(m_playerController, &PlayerController::inventoryPressed, this, &Engine::onInventoryClicked);

    connect(m_playerController, &PlayerController::escape, this, &Engine::onEscape);
    connect(m_playerController, &PlayerController::beam, this, &Engine::onBeam);
    //connect(m_playerController, &PlayerController::shoot, this, &Engine::onShoot);

    qCDebug(dcEngine()) << "Created engine" << thread();
}

Engine::State Engine::state() const
{
    return m_state;
}

QRectF Engine::viewWindow() const
{
    return m_viewWindow;
}

void Engine::setViewWindow(const QRectF &viewWindow)
{
    if (m_viewWindow == viewWindow)
        return;

    m_viewWindow = viewWindow;
    emit viewWindowChanged(m_viewWindow);

    m_activeItems->setViewFilter(m_viewWindow);
    m_activeCharacters->setViewFilter(m_viewWindow);
    m_activeEnemies->setViewFilter(m_viewWindow);
}

Character *Engine::player() const
{
    return m_player;
}

GameItemsProxy *Engine::activeItems() const
{
    return m_activeItems;
}

GameItemsProxy *Engine::activeCharacters() const
{
    return m_activeCharacters;
}

GameItemsProxy *Engine::activeEnemies() const
{
    return m_activeEnemies;
}

PlayerController *Engine::playerController() const
{
    return m_playerController;
}

GameItem *Engine::playerFocusItem() const
{
    return m_playerFocusItem;
}

Conversation *Engine::currentConversation() const
{
    return m_currentConversation;
}

ChestItem *Engine::currentChestItem() const
{
    return m_currentChestItem;
}

GameItems *Engine::currentPlunderItems() const
{
    return m_currentPlunderItems;
}

bool Engine::loaded() const
{
    return m_loaded;
}

bool Engine::loading() const
{
    return m_loading;
}

void Engine::pauseGame()
{
    setState(StatePaused);
}

void Engine::resumeGame()
{
    setState(StateRunning);
}

void Engine::inventoryClicked()
{
    onInventoryClicked();
}

void Engine::unlockLeftClicked()
{
    if (!m_currentChestItem)
        return;

    onLeftClicked();
}

void Engine::unlockRightClicked()
{
    if (!m_currentChestItem)
        return;

    onRightClicked();
}

void Engine::giveUpUnlocking()
{
    qCDebug(dcEngine()) << "Give up unlocking.";
    setCurrentChestItem(nullptr);
    setCurrentPlunderItems(nullptr);
    setState(StateRunning);
}

void Engine::finishPlunder()
{
    qCDebug(dcEngine()) << "Finish plundering";
    setCurrentChestItem(nullptr);
    setCurrentPlunderItems(nullptr);
    setState(StateRunning);
}

void Engine::useInventoryItem(const QString &itemId)
{
    GameItem *item = m_player->inventory()->takeLastGameItem(itemId);
    if (!item) {
        qCWarning(dcEngine()) << "There is no item with the given id in the inventory.";
        return;
    }

    qCDebug(dcEngine()) << "Use inventory item" << item;
    item->deleteLater();

    switch (item->itemType()) {
    case GameItem::TypePlant: {
        PlantItem *plant = qobject_cast<PlantItem *>(item);
        m_player->setMana(m_player->mana() + plant->mana());
        m_player->setHealth(m_player->health() + plant->healing());
        break;
    }
    default:
        qCWarning(dcEngine()) << "Unhandled use for game item" << item;
    }
}

void Engine::performHitAttack(Character *attacker, Character *victim, int damage)
{
    qCDebug(dcEngine()) << "Hit attack" << damage << attacker->name() << "-->" << victim->name();
    victim->setHealth(victim->health() - damage);
}

void Engine::performShootImpact(Character *attacker, Character *victim, int damage)
{
    qCDebug(dcEngine()) << "Shoot attack" << damage << attacker->name() << "-->" << victim->name();
    victim->setHealth(victim->health() - damage);
}

void Engine::performBurnDamage(Character *victim, int damage)
{
    qCDebug(dcEngine()) << "Burn damage" << victim << damage;
    victim->setHealth(victim->health() - damage);
}

void Engine::takeItem(GameItems *gameItems, GameItem *item)
{
    qCDebug(dcEngine()) << "Move" << item << "to inventory";

    // Get the list of all matching items
    QList<GameItem *> targetItems;
    foreach (GameItem * targetItem, gameItems->gameItems()) {
        if (targetItem->itemId() == item->itemId()) {
            targetItems.append(targetItem);
        }
    }

    // If there is no item left
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

void Engine::takeAllItems(GameItems *gameItems)
{
    foreach (GameItem *item, gameItems->gameItems()) {
        gameItems->removeGameItem(item);
        m_player->inventory()->addGameItem(item);
    }
}

void Engine::setState(Engine::State state)
{
    if (m_state == state)
        return;

    qCDebug(dcEngine()) << "--> State changed:" << state;
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

QPoint Engine::currentPlayerPosition() const
{
    return m_currentPlayerPosition;
}

Field *Engine::currentPlayerField() const
{
    return m_currentPlayerField;
}

DataManager *Engine::dataManager() const
{
    return m_dataManager;
}

void Engine::setCurrentPlayerPosition(const QPoint &currentPosition)
{
    if (m_currentPlayerPosition == currentPosition)
        return;

    qCDebug(dcEngineData()) << "Current player position changed" << currentPosition;
    m_currentPlayerPosition = currentPosition;
    emit currentPlayerPositionChanged(m_currentPlayerPosition);
}

void Engine::setCurrentPlayerField(Field *field)
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

void Engine::setPlayerFocusItem(GameItem *focusItem)
{
    if (m_playerFocusItem == focusItem)
        return;

    if (focusItem) {
        qCDebug(dcEngineData()) << "Player focus item" << focusItem;
    } else {
        qCDebug(dcEngineData()) << "Player has no item in focus";
    }

    m_playerFocusItem = focusItem;
    emit playerFocusItemChanged(m_playerFocusItem);
}

void Engine::setLoaded(bool loaded)
{
    if (m_loaded == loaded)
        return;

    qCDebug(dcEngine()) << "Loaded changed:" << loaded;
    m_loaded = loaded;
    emit loadedChanged(m_loaded);
}

void Engine::setLoading(bool loading)
{
    if (m_loading == loading)
        return;

    qCDebug(dcEngine()) << "Loading changed:" << loading;
    m_loading = loading;
    emit loadingChanged(m_loading);
}

void Engine::setPlayer(Character *player)
{
    // Remove old player
    if (m_player) {
        m_player->setIsPlayer(false);
        disconnect(m_player, &Character::positionChanged, this, &Engine::onPlayerPositionChanged);
        m_player = nullptr;
    }

    if (player) {
        qCDebug(dcEngine()) << "Set player" << player;
    } else {
        qCDebug(dcEngine()) << "Clear player";
    }

    m_player = player;
    emit playerChanged(m_player);

    setPlayerFocusItem(nullptr);

    foreach (GameItem *visibleItem, m_playerVisibleItems) {
        visibleItem->setPlayerFocus(false);
    }

    if (m_player) {
        m_player->setIsPlayer(true);
        m_player->setMovable(true);
        m_playerController->setPlayer(m_player);
        connect(m_player, &Character::positionChanged, this, &Engine::onPlayerPositionChanged);
//        evaluatePlayerFocus();
//        onPlayerPositionChanged();
    }

}

void Engine::setCurrentConversation(Conversation *conversation)
{
    if (m_currentConversation == conversation)
        return;

    qCDebug(dcEngineData()) << "Set current conversation" << conversation;
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

void Engine::setCurrentChestItem(ChestItem *chestItem)
{
    if (m_currentChestItem == chestItem)
        return;

    m_currentChestItem = chestItem;
    emit currentChestItemChanged(m_currentChestItem);

    if (m_currentChestItem) {
        qCDebug(dcEngineData()) << "Set current chest item" << m_currentChestItem << "containing" << m_currentChestItem->items()->gameItems().count() << "items";
        setCurrentPlunderItems(m_currentChestItem->items());
        if (m_currentChestItem->locked()) {
            qCDebug(dcEngine()) << "The chest is locked. Show unlock screen";
            setState(StateUnlocking);
        } else {
            qCDebug(dcEngine()) << "The chest is not locked. Show plunder screen";
            setState(StatePlunder);
        }
    } else {
        qCDebug(dcEngine()) << "Reset chest item.";
        setCurrentPlunderItems(nullptr);
        m_player->setMovable(true);
    }
}

void Engine::setCurrentPlunderItems(GameItems *plunderItems)
{
    if (m_currentPlunderItems == plunderItems)
        return;

    m_currentPlunderItems = plunderItems;
    emit currentPlunderItemsChanged(m_currentPlunderItems);
}

void Engine::doPlayerMovement()
{
    if (!m_player)
        return;

    if (!m_player->movable())
        return;

    m_player->setMovementVector(m_playerController->movementVector());
}

void Engine::doEnemiesMovement()
{
    for (int i = 0; i < m_activeEnemies->count(); i++) {
        Enemy *enemy = qobject_cast<Enemy *>(m_activeEnemies->get(i));
        if (!enemy->movable())
            continue;

        enemy->onTick();
    }
}

void Engine::doCharactersMovement()
{
    for (int i = 0; i < m_activeCharacters->count(); i++) {
        Character *character = qobject_cast<Character *>(m_activeCharacters->get(i));
        if (!character->movable())
            continue;

        character->onTick();
    }
}

void Engine::evaluatePlayerFocus()
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
            closestItem.second = CollisionDetector::calculateCenterDistance(m_player, visibleItem);
            continue;
        } else {
            double distance = CollisionDetector::calculateCenterDistance(m_player, visibleItem);
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

void Engine::pickItem(GameItem *item)
{
    qCDebug(dcEngine()) << "Pick up item" << item;

    item->setPlayerFocus(false);
    item->setPlayerVisible(false);

    m_dataManager->items()->removeGameItem(item);
    qCDebug(dcEngineData()) << "Picked item and add it to the inventory" << item;
    m_player->inventory()->addGameItem(item);
}

void Engine::onPlayerPositionChanged()
{
    // Calculate the current field on the map
    int x = static_cast<int>(m_player->centerPosition().x());
    int y = static_cast<int>(m_player->centerPosition().y());
    setCurrentPlayerPosition(QPoint(x, y));
    //setCurrentPlayerField(m_map->getField(x, y));
}

void Engine::onDataManagerStateChanged(DataManager::State state)
{
    switch (state) {
    case DataManager::StateStarting:
    case DataManager::StateLoading:
        setState(StateLoading);
        setCurrentChestItem(nullptr);
        setCurrentConversation(nullptr);
        setCurrentPlunderItems(nullptr);
        setPlayerFocusItem(nullptr);
        setPlayer(nullptr);
        setLoaded(false);
        setLoading(true);
        break;
    case DataManager::StateIdle:
        if (!m_dataManager->player())
            return;

        setPlayer(m_dataManager->player());
        m_player->setName(Game::instance()->settings()->playerName());
        m_playerController->setControlMode(Game::instance()->settings()->controlMode());
        doPlayerMovement();
        evaluatePlayerFocus();
        setLoading(false);
        setLoaded(true);
        setState(StateRunning);
        break;
    default:
        qCWarning(dcEngine()) << "Unhandled state" << state;
        break;
    }
}

void Engine::onGameItemActiveChanged(GameItem *item, bool active)
{
    if (active) {
        qCDebug(dcEngineData()) << "[+] Item changed to active" << item;
        connect(item, &GameItem::playerVisibleChanged, this, &Engine::onItemPlayerVisibleChanged);
        connect(item, &GameItem::playerOnItemChanged, this, &Engine::onItemPlayerOnItemChanged);

        switch (item->itemType()) {
        case GameItem::TypeCharacter:
            connect(qobject_cast<Character *>(item), &Character::killed, this, &Engine::onCharacterKilled);
            break;
        case GameItem::TypeEnemy:
            connect(qobject_cast<Enemy *>(item), &Enemy::killed, this, &Engine::onEnemyKilled);
            break;
        default:
            break;
        }
    } else {
        qCDebug(dcEngineData()) << "[-] Item changed to inactive" << item;
        disconnect(item, &GameItem::playerVisibleChanged, this, &Engine::onItemPlayerVisibleChanged);
        disconnect(item, &GameItem::playerOnItemChanged, this, &Engine::onItemPlayerOnItemChanged);

        switch (item->itemType()) {
        case GameItem::TypeCharacter:
            disconnect(qobject_cast<Character *>(item), &Character::killed, this, &Engine::onCharacterKilled);
            break;
        case GameItem::TypeEnemy:
            disconnect(qobject_cast<Enemy *>(item), &Enemy::killed, this, &Engine::onEnemyKilled);
            break;
        default:
            break;
        }
    }

    item->setActive(active);
}

void Engine::onItemPlayerVisibleChanged(bool playerVisible)
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

void Engine::onItemPlayerOnItemChanged(bool playerOnItem)
{
    GameItem *item = qobject_cast<GameItem *>(sender());
    if (playerOnItem) {
        switch (item->itemType()) {
        case GameItem::TypeEnemy: {
            Enemy *enemy = qobject_cast<Enemy *>(item);
            if (enemy->touchDamage() && enemy->alive()) {
                qCDebug(dcEngine()) << "Player touch damage" << enemy->touchDamage() << "from" << enemy->name();
                m_player->setHealth(m_player->health() - enemy->touchDamage());
            }
            break;
        }
        default:
            break;
        }
    }
}

void Engine::onEnemyKilled()
{
    Enemy *enemy = qobject_cast<Enemy *>(sender());
    enemy->setInteraction(GameItem::InteractionPlunder);
    enemy->setMovable(false);
}

void Engine::onCharacterKilled()
{
    Character *character = qobject_cast<Character *>(sender());
    character->setInteraction(GameItem::InteractionPlunder);
    character->setMovable(false);
}

void Engine::onPrimaryActionPressedChanged(bool pressed)
{
    qCDebug(dcEngine()) << "Primary action" << (pressed ? "pressed" : "released");

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
            qCDebug(dcEngine()) << "Perform interaction" << m_playerFocusItem->interaction() << m_playerFocusItem;
            if (m_playerFocusItem->interaction() == GameItem::InteractionPick) {
                m_playerFocusItem->performInteraction();
                pickItem(m_playerFocusItem);
                m_playerFocusItem = nullptr;
                evaluatePlayerFocus();
            }
            break;
        case GameItem::TypeChest:
            qCDebug(dcEngine()) << "Perform interaction" << m_playerFocusItem->interaction() << m_playerFocusItem;
            if (m_playerFocusItem->interaction() == GameItem::InteractionOpen) {
                ChestItem *chestItem = qobject_cast<ChestItem *>(m_playerFocusItem);
                setCurrentChestItem(chestItem);
            }
            break;
        case GameItem::TypeCharacter:
            qCDebug(dcEngine()) << "Perform interaction" << m_playerFocusItem->interaction() << m_playerFocusItem;
            if (m_playerFocusItem->interaction() == GameItem::InteractionTalk) {
                m_playerFocusItem->performInteraction();
                Character *character = qobject_cast<Character *>(m_playerFocusItem);
                character->lookToPoint(m_player->position());
                character->setMovable(false);
                // FIXME: load conversation

                Conversation *conversation = new Conversation(character, DataLoader::loadJsonData(":/dialogs/test-dialog.json"));
                connect(conversation, &Conversation::conversationFinished, this, &Engine::onCurrentConversationFinished);

                if (m_currentConversation) {
                    m_currentConversation->deleteLater();
                    m_currentConversation = nullptr;
                }

                // Start conversation
                setCurrentConversation(conversation);
            } else if (m_playerFocusItem->interaction() == GameItem::InteractionPlunder) {
                Character *character = qobject_cast<Character *>(m_playerFocusItem);
                setCurrentPlunderItems(character->inventory());
                setState(StatePlunder);
            }
            break;
        case GameItem::TypeEnemy:
            qCDebug(dcEngine()) << "Perform enemy interaction" << m_playerFocusItem->interaction() << m_playerFocusItem;
            if (m_playerFocusItem->interaction() == GameItem::InteractionPlunder) {
                Enemy *enemy = qobject_cast<Enemy *>(m_playerFocusItem);
                setCurrentPlunderItems(enemy->inventory());
                setState(StatePlunder);
            }

            break;
        default:
            qCWarning(dcEngine()) << "Unhandled action on item" << m_playerFocusItem;
            break;
        }
    }
}

void Engine::onSecondaryActionPressedChanged(bool pressed)
{
    switch (m_state) {
    case StateConversation:
        if (m_currentConversation && pressed) m_currentConversation->confirmPressed();
        break;
    case StateRunning:
        if (!pressed || !m_player)
            return;

        qCDebug(dcEngine) << "Secondary action pressed";
        switch (m_player->armed()) {
        case Character::ArmedNone:
            break;
        case Character::ArmedWeapon:
            m_player->setHitNumber(m_player->hitNumber() + 1);
            break;
        case Character::ArmedFirearm:
            m_player->setShootNumber(m_player->shootNumber() + 1);
            break;
        }
        break;
    default:
        qCDebug(dcEngine()) << "Secondary action" << (pressed ? "pressed" : "released");
        break;
    }
}

void Engine::onLeftClicked()
{
    switch (m_state) {
    case StateUnlocking:
        m_currentChestItem->unlockLeftMovement();
        if (!m_currentChestItem->locked()) {
            qCDebug(dcEngine()) << "Chest unlocked!";
            setState(StatePlunder);
        }
        break;
    default:
        break;
    }
}

void Engine::onRightClicked()
{
    switch (m_state) {
    case StateUnlocking:
        m_currentChestItem->unlockRightMovement();
        if (!m_currentChestItem->locked()) {
            qCDebug(dcEngine()) << "Chest unlocked!";
            setState(StatePlunder);
        }
        break;
    default:
        break;
    }
}

void Engine::onForwardClicked()
{
    switch (m_state) {
    case StateConversation:
        if (m_currentConversation) m_currentConversation->upPressed();
        break;
    default:
        break;
    }
}

void Engine::onBackwardClicked()
{
    switch (m_state) {
    case StateConversation:
        if (m_currentConversation) m_currentConversation->downPressed();
        break;
    default:
        break;
    }
}

void Engine::onInventoryClicked()
{
    qCDebug(dcEngine()) << "Inventory clicked";
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

void Engine::onCurrentConversationFinished()
{
    qCDebug(dcEngine()) << "Conversation finished";
    m_currentConversation->character()->setMovable(true);
    m_currentConversation->deleteLater();
    setCurrentConversation(nullptr);
    setState(StateRunning);
}

void Engine::onShoot()
{
    switch (m_state) {
    case StateRunning:
        if (!m_player->firearm()) {
            qCDebug(dcEngine()) << "Player not shooting, no firearm selected.";
            break;
        }
        qCDebug(dcEngine()) << "Player shoot with" << m_player->firearm();
        if (m_player) emit m_player->shoot();
        break;
    default:
        break;
    }
}

void Engine::onEscape()
{
    qCDebug(dcEngine()) << "Escape clicked";
    switch (m_state) {
    case StateInventory:
    case StatePaused:
        setState(StateRunning);
        break;
    case StateRunning:
        setState(StatePaused);
        break;
    case StatePlunder:
        finishPlunder();
        break;
    case StateUnlocking:
        giveUpUnlocking();
        break;
    default:
        break;
    }
}

void Engine::onBeam()
{
    if (!m_playerFocusItem)
        return;

    if (m_playerFocusItem->itemType() != GameItem::TypeCharacter)
        return;

    qCDebug(dcEngine()) << "Beam player into" << m_playerFocusItem;
    setPlayer(qobject_cast<Character *>(m_playerFocusItem));
}

void Engine::tick()
{
    doPlayerMovement();
    doEnemiesMovement();
    doCharactersMovement();

    // Do other evaluation stuff

    emit enginePostTick();
}

void Engine::resetEngine()
{
    qCDebug(dcEngine()) << "Reset the game engine";

    setCurrentPlayerPosition(QPoint());
    setCurrentPlayerField(nullptr);
    m_playerVisibleItems.clear();
    setPlayerFocusItem(nullptr);
    setLoaded(false);
    setLoading(false);
    setCurrentConversation(nullptr);
    setCurrentChestItem(nullptr);
    setCurrentPlunderItems(nullptr);

    m_dataManager->resetData();
    setPlayer(nullptr);

    setState(StateUnitialized);
}
