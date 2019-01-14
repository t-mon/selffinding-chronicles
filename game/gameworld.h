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
#include "items/gameitems.h"
#include "playercontroller.h"
#include "conversation/conversation.h"

class CollisionDetector;

class GameWorld : public Fields
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QSize boundingSize READ boundingSize WRITE setBoundingSize NOTIFY boundingSizeChanged)
    Q_PROPERTY(State state READ state NOTIFY stateChanged)

    Q_PROPERTY(Map *map READ map CONSTANT)
    Q_PROPERTY(PlayerController *playerController READ playerController CONSTANT)

    Q_PROPERTY(Character *player READ player CONSTANT)
    Q_PROPERTY(GameItem *playerFocusItem READ playerFocusItem NOTIFY playerFocusItemChanged)
    Q_PROPERTY(GameItems *gameItems READ gameItems CONSTANT)
    Q_PROPERTY(GameItems *enemyItems READ enemyItems CONSTANT)
    Q_PROPERTY(GameItems *characterItems READ characterItems CONSTANT)

    Q_PROPERTY(Conversation *currentConversation READ currentConversation NOTIFY currentConversationChanged)
    Q_PROPERTY(ChestItem *currentChestItem READ currentChestItem NOTIFY currentChestItemChanged)
    Q_PROPERTY(GameItems *currentPlunderItems READ currentPlunderItems NOTIFY currentPlunderItemsChanged)

    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    Q_PROPERTY(Field* currentPlayerField READ currentPlayerField NOTIFY currentPlayerFieldChanged)
    Q_PROPERTY(QPoint currentPlayerPosition READ currentPlayerPosition NOTIFY currentPlayerPositionChanged)
    Q_PROPERTY(QPoint currentViewOffset READ currentViewOffset WRITE setCurrentViewOffset NOTIFY currentViewOffsetChanged)

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

    explicit GameWorld(QObject *parent = nullptr);
    ~GameWorld() override = default;

    State state() const;

    QSize size() const;
    void setSize(const QSize &size);

    QSize boundingSize() const;
    void setBoundingSize(const QSize &boundingSize);

    QPoint currentViewOffset() const;
    void setCurrentViewOffset(const QPoint &currentViewOffset);

    QPoint currentPlayerPosition() const;
    Field *currentPlayerField() const;

    Map *map() const;
    Character *player() const;
    GameItems *gameItems() const;
    GameItems *characterItems() const;
    GameItems *enemyItems() const;
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

    Q_INVOKABLE void loadMap(const QString &fileName);

    Q_INVOKABLE void giveUpUnlocking();
    Q_INVOKABLE void finishPlunder();

    Q_INVOKABLE void performHitAttack(Character *attacker, Character *victim, int damage);
    Q_INVOKABLE void performShootImpact(Character *attacker, Character *victim, int damage);
    Q_INVOKABLE void performBurnDamage(Character *victim, int damage);

    Q_INVOKABLE void takeItem(GameItems *gameItems, GameItem *item);
    Q_INVOKABLE void takeAllItems(GameItems *gameItems);

private:
    State m_state = StateUnitialized;
    QSize m_size;
    QSize m_boundingSize;

    Map *m_map = nullptr;
    Character *m_player = nullptr;
    GameItems *m_gameItems = nullptr;
    GameItems *m_enemyItems = nullptr;
    GameItems *m_characterItems = nullptr;
    PlayerController *m_playerController = nullptr;
    CollisionDetector *m_collisionDetector = nullptr;
    Conversation *m_currentConversation = nullptr;
    ChestItem *m_currentChestItem = nullptr;
    GameItems *m_currentPlunderItems = nullptr;

    // View properties
    QPoint m_currentPlayerPosition;
    QPoint m_currentViewOffset;

    Field *m_currentPlayerField = nullptr;
    QList<Field *> m_fieldsInRange;
    QList<GameItem *> m_playerVisibleItems;
    GameItem *m_playerFocusItem = nullptr;
    GameItem *m_playerCurrentlyOnItem = nullptr;

    // Map loading
    QFutureWatcher<void> *m_loadingWatcher = nullptr;
    bool m_loaded = false;
    bool m_loading = false;

    // Set methods
    void setState(State state);
    void setCurrentPlayerPosition(const QPoint &currentPosition);
    void setCurrentPlayerField(Field *field);
    void setPlayerFocusItem(GameItem *focusItem);
    void setLoaded(bool loaded);
    void setLoading(bool loading);
    void setCurrentConversation(Conversation *conversation);
    void setCurrentChestItem(ChestItem *chestItem);
    void setCurrentPlunderItems(GameItems *plunderItems);

    // Move methods
    void doPlayerMovement();

    // Helper methods
    Field *getFieldFromPosition(const QPointF position) const;
    void evaluatePlayerFocus();

    void pickItem(GameItem *item);

signals:
    void worldPostTick();
    void stateChanged(State state);
    void sizeChanged(const QSize &size);
    void boundingSizeChanged(const QSize &boundingSize);
    void loadingChanged(bool loading);
    void loadedChanged(bool loaded);

    void currentPlayerFieldChanged(Field *currentPlayerField);
    void currentPlayerPositionChanged(const QPoint &currentPlayerPosition);
    void currentViewOffsetChanged(const QPoint &currentViewOffset);
    void playerFocusItemChanged(GameItem *playerFocusItem);
    void currentConversationChanged(Conversation *conversation);
    void currentChestItemChanged(ChestItem *chestItem);
    void currentPlunderItemsChanged(GameItems *plunderItems);

private slots:
    void onPlayerPositionChanged();
    void onLoadingFinished();

    void onItemPlayerVisibleChanged(bool playerVisible);
    void onItemPlayerOnItemChanged(bool playerOnItem);
    void onEnemyKilled();

    void onPrimaryActionPressedChanged(bool pressed);
    void onSecondaryActionPressedChanged(bool pressed);
    void onLeftClicked();
    void onRightClicked();
    void onForwardClicked();
    void onBackwardClicked();
    void onInventoryClicked();
    void onCurrentConversationFinished();

public slots:
    void tick();

};

#endif // WORLD_H
