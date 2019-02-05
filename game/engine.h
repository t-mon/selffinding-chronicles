#ifndef WORLD_H
#define WORLD_H

#include <QSize>
#include <QPoint>
#include <QObject>
#include <QFuture>
#include <QVector2D>
#include <QFutureWatcher>

#include "map.h"
#include "fields.h"
#include "datamanager.h"
#include "items/gameitems.h"
#include "playercontroller.h"
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
    Q_PROPERTY(GameItemsProxy *activeItems READ activeItems CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeEnemies READ activeEnemies CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeCharacters READ activeCharacters CONSTANT)

    Q_PROPERTY(Conversation *currentConversation READ currentConversation NOTIFY currentConversationChanged)
    Q_PROPERTY(ChestItem *currentChestItem READ currentChestItem NOTIFY currentChestItemChanged)
    Q_PROPERTY(GameItems *currentPlunderItems READ currentPlunderItems NOTIFY currentPlunderItemsChanged)

    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    Q_PROPERTY(Field* currentPlayerField READ currentPlayerField NOTIFY currentPlayerFieldChanged)
    Q_PROPERTY(QPoint currentPlayerPosition READ currentPlayerPosition NOTIFY currentPlayerPositionChanged)

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
        StatePlunder
    };
    Q_ENUM(State)

    explicit Engine(QObject *parent = nullptr);
    ~Engine() override = default;

    State state() const;

    QRectF viewWindow() const;
    void setViewWindow(const QRectF &viewWindow);

    QPoint currentPlayerPosition() const;
    Field *currentPlayerField() const;

    DataManager *dataManager() const;
    Character *player() const;
    GameItemsProxy *activeItems() const;
    GameItemsProxy *activeCharacters() const;
    GameItemsProxy *activeEnemies() const;

    PlayerController *playerController() const;

    GameItem *playerFocusItem() const;
    Conversation *currentConversation() const;
    ChestItem *currentChestItem() const;
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
    Q_INVOKABLE void useInventoryItem(const QString &itemId);

    Q_INVOKABLE void performHitAttack(Character *attacker, Character *victim, int damage);
    Q_INVOKABLE void performShootImpact(Character *attacker, Character *victim, int damage);
    Q_INVOKABLE void performBurnDamage(Character *victim, int damage);

    Q_INVOKABLE void takeItem(GameItems *gameItems, GameItem *item);
    Q_INVOKABLE void takeAllItems(GameItems *gameItems);

private:
    State m_state = StateUnitialized;
    QRectF m_viewWindow;

    DataManager *m_dataManager = nullptr;

    Character *m_player = nullptr;
    PlayerController *m_playerController = nullptr;

    GameItemsProxy *m_activeItems = nullptr;
    GameItemsProxy *m_activeEnemies = nullptr;
    GameItemsProxy *m_activeCharacters = nullptr;

    Conversation *m_currentConversation = nullptr;
    ChestItem *m_currentChestItem = nullptr;
    GameItems *m_currentPlunderItems = nullptr;

    // View properties
    QPoint m_currentPlayerPosition;
    Field *m_currentPlayerField = nullptr;

    QList<GameItem *> m_playerVisibleItems;
    GameItem *m_playerFocusItem = nullptr;

    // Map loading
    bool m_loaded = false;
    bool m_loading = false;

    // Set methods
    void setState(State state);
    void setCurrentPlayerPosition(const QPoint &currentPosition);
    void setCurrentPlayerField(Field *field);
    void setPlayerFocusItem(GameItem *focusItem);
    void setLoaded(bool loaded);
    void setLoading(bool loading);
    void setPlayer(Character *player);

    void setCurrentConversation(Conversation *conversation);
    void setCurrentChestItem(ChestItem *chestItem);
    void setCurrentPlunderItems(GameItems *plunderItems);

    // Move methods
    void doPlayerMovement();

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
    void currentPlayerPositionChanged(const QPoint &currentPlayerPosition);
    void currentViewOffsetChanged(const QPoint &currentViewOffset);
    void playerFocusItemChanged(GameItem *playerFocusItem);
    void currentConversationChanged(Conversation *conversation);
    void currentChestItemChanged(ChestItem *chestItem);
    void currentPlunderItemsChanged(GameItems *plunderItems);

private slots:
    void onPlayerPositionChanged();
    void onDataManagerStateChanged(DataManager::State state);

    void onGameItemActiveChanged(GameItem *item, bool active);

    void onItemPlayerVisibleChanged(bool playerVisible);
    void onItemPlayerOnItemChanged(bool playerOnItem);
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
