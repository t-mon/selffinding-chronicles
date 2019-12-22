#ifndef WORLD_H
#define WORLD_H

#include <QSizeF>
#include <QPointF>
#include <QObject>
#include <QFuture>
#include <QVector2D>
#include <QFutureWatcher>
#include <QElapsedTimer>

#include "map.h"
#include "fields.h"
#include "datamanager.h"
#include "gameobjects.h"
#include "gameobjectsproxy.h"
#include "items/gameitems.h"
#include "playercontroller.h"
#include "items/teleporteritem.h"
#include "teleportationhandler.h"
#include "items/weatherareaproxy.h"
#include "conversation/conversation.h"

class CollisionDetector;

class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QRectF viewWindow READ viewWindow WRITE setViewWindow NOTIFY viewWindowChanged)

    Q_PROPERTY(PlayerController *playerController READ playerController CONSTANT)
    Q_PROPERTY(Character *player READ player NOTIFY playerChanged)
    Q_PROPERTY(GameItem *playerFocusItem READ playerFocusItem NOTIFY playerFocusItemChanged)

    Q_PROPERTY(DataManager *dataManager READ dataManager CONSTANT)
    Q_PROPERTY(GameObjectsProxy *activeObjects READ activeObjects CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeItems READ activeItems CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeEnemies READ activeEnemies CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeCharacters READ activeCharacters CONSTANT)
    Q_PROPERTY(WeatherAreaProxy *activeWeatherAreas READ activeWeatherAreas CONSTANT)

    Q_PROPERTY(Conversation *currentConversation READ currentConversation NOTIFY currentConversationChanged)
    Q_PROPERTY(ChestItem *currentChestItem READ currentChestItem NOTIFY currentChestItemChanged)
    Q_PROPERTY(LiteratureItem *currentLiteratureItem READ currentLiteratureItem NOTIFY currentLiteratureItemChanged)
    Q_PROPERTY(GameItems *currentPlunderItems READ currentPlunderItems NOTIFY currentPlunderItemsChanged)

    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    Q_PROPERTY(Field* currentPlayerField READ currentPlayerField NOTIFY currentPlayerFieldChanged)
    Q_PROPERTY(QPointF currentPlayerPosition READ currentPlayerPosition NOTIFY currentPlayerPositionChanged)

public:
    enum State {
        StateUnitialized,
        StateLoading,
        StateRunning,
        StatePaused,
        StateInventory,
        StateTrade,
        StateConversation,
        StateUnlocking,
        StatePlunder,
        StateRead,
        StateTeleport
    };
    Q_ENUM(State)

    explicit Engine(QObject *parent = nullptr);
    ~Engine() override = default;

    State state() const;

    QRectF viewWindow() const;
    void setViewWindow(const QRectF &viewWindow);

    QPointF currentPlayerPosition() const;
    Field *currentPlayerField() const;

    DataManager *dataManager() const;
    Character *player() const;
    GameObjectsProxy *activeObjects() const;
    GameItemsProxy *activeItems() const;
    GameItemsProxy *activeCharacters() const;
    GameItemsProxy *activeEnemies() const;
    WeatherAreaProxy *activeWeatherAreas() const;

    PlayerController *playerController() const;

    GameItem *playerFocusItem() const;
    Conversation *currentConversation() const;
    ChestItem *currentChestItem() const;
    LiteratureItem *currentLiteratureItem() const;
    TeleporterItem *currentTeleportItem() const;
    GameItems *currentPlunderItems() const;

    bool loaded() const;
    bool loading() const;

    Q_INVOKABLE void pauseGame();
    Q_INVOKABLE void resumeGame();
    Q_INVOKABLE void inventoryClicked();
    Q_INVOKABLE void unlockLeftClicked();
    Q_INVOKABLE void unlockRightClicked();

    Q_INVOKABLE void giveUpUnlocking();
    Q_INVOKABLE void finishPlunder();
    Q_INVOKABLE void finishReading();
    Q_INVOKABLE void useInventoryItem(const QString &itemId);

    Q_INVOKABLE void performHitAttack(Character *attacker, Character *victim, int damage);
    Q_INVOKABLE void performShootImpact(Character *attacker, Character *victim, int damage);
    Q_INVOKABLE void performBurnDamage(Character *victim, int damage);

    Q_INVOKABLE void takeItem(GameItems *gameItems, GameItem *item);
    Q_INVOKABLE void takeAllItems(GameItems *gameItems);

private:
    State m_state = StateUnitialized;

    QRectF m_viewWindow = QRectF(0, 0, 10, 10);

    DataManager *m_dataManager = nullptr;
    QElapsedTimer m_loadingTimer;

    Character *m_player = nullptr;
    PlayerController *m_playerController = nullptr;

    GameObjectsProxy *m_activeObjects = nullptr;
    GameItemsProxy *m_activeItems = nullptr;
    GameItemsProxy *m_activeEnemies = nullptr;
    GameItemsProxy *m_activeCharacters = nullptr;
    WeatherAreaModel *m_weatherAreaModel = nullptr;
    WeatherAreaProxy *m_weatherAreaProxy = nullptr;

    TeleportationHandler *m_teleportationHandler = nullptr;

    Conversation *m_currentConversation = nullptr;
    Character *m_currentConversationCharacter = nullptr;
    ChestItem *m_currentChestItem = nullptr;
    LiteratureItem *m_currentLiteratureItem = nullptr;
    GameItems *m_currentPlunderItems = nullptr;

    bool m_keepInventoryOpen = false;

    // View properties
    QPointF m_currentPlayerPosition;
    Field *m_currentPlayerField = nullptr;

    QList<GameItem *> m_playerVisibleItems;
    GameItem *m_playerFocusItem = nullptr;

    // Map loading
    bool m_loaded = false;
    bool m_loading = false;

    // Set methods
    void setState(State state);
    void setCurrentPlayerPosition(const QPointF &currentPosition);
    void setCurrentPlayerField(Field *field);
    void setPlayerFocusItem(GameItem *focusItem);
    void setLoaded(bool loaded);
    void setLoading(bool loading);
    void setPlayer(Character *player);

    void setCurrentConversation(Conversation *conversation);
    void setCurrentChestItem(ChestItem *chestItem);
    void setCurrentLiteratureItem(LiteratureItem *literatureItem);
    void setCurrentPlunderItems(GameItems *plunderItems);

    // Move methods
    void doPlayerMovement();
    void doEnemiesMovement();
    void doCharactersMovement();

    // Helper methods
    void evaluatePlayerFocus();
    void pickItem(GameItem *item);

signals:
    void enginePostTick();

    void stateChanged(State state);

    void viewWindowChanged(const QRectF &viewWindow);
    void loadingChanged(bool loading);
    void loadedChanged(bool loaded);

    void playerChanged(Character *player);
    void currentPlayerFieldChanged(Field *currentPlayerField);
    void currentPlayerPositionChanged(const QPointF &currentPlayerPosition);
    void currentViewOffsetChanged(const QPointF &currentViewOffset);
    void playerFocusItemChanged(GameItem *playerFocusItem);
    void currentConversationChanged(Conversation *conversation);
    void currentChestItemChanged(ChestItem *chestItem);
    void currentLiteratureItemChanged(LiteratureItem *literatureItem);
    void currentPlunderItemsChanged(GameItems *plunderItems);

private slots:
    void onPlayerPositionChanged();
    void onDataManagerStateChanged(DataManager::State state);

    // Item slots
    void onGameObjectActiveChanged(GameObject *object, bool active);
    void onGameItemActiveChanged(GameItem *item, bool active);
    void onItemPlayerVisibleChanged(bool playerVisible);
    void onItemPlayerOnItemChanged(bool playerOnItem);
    void onCharacterKilled();
    void onEnemyKilled();

    // Playercontroller
    void onPrimaryActionPressedChanged(bool pressed);
    void onSecondaryActionPressedChanged(bool pressed);
    void onLeftClicked();
    void onRightClicked();
    void onForwardClicked();
    void onBackwardClicked();
    void onInventoryClicked();
    void onCurrentConversationFinished();
    void onShoot();
    void onEscape();
    void onBeam();

public slots:
    void tick();
    void resetEngine();

};

#endif // WORLD_H
