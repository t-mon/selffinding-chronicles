#ifndef WORLD_H
#define WORLD_H

#include <QSize>
#include <QPoint>
#include <QObject>
#include <QFuture>
#include <QVector2D>
#include <QFutureWatcher>

#include "map.h"
#include "player.h"
#include "fields.h"
#include "items/gameitems.h"
#include "playercontroller.h"

class CollisionDetector;

class World : public Fields
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QSize boundingSize READ boundingSize WRITE setBoundingSize NOTIFY boundingSizeChanged)

    Q_PROPERTY(Map *map READ map CONSTANT)
    Q_PROPERTY(Player *player READ player CONSTANT)
    Q_PROPERTY(GameItems *gameItems READ gameItems CONSTANT)
    Q_PROPERTY(PlayerController *playerController READ playerController CONSTANT)

    Q_PROPERTY(bool loaded READ loaded NOTIFY loadedChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)

    Q_PROPERTY(Field* currentPlayerField READ currentPlayerField NOTIFY currentPlayerFieldChanged)
    Q_PROPERTY(QPoint currentPlayerPosition READ currentPlayerPosition NOTIFY currentPlayerPositionChanged)
    Q_PROPERTY(QPoint currentViewOffset READ currentViewOffset WRITE setCurrentViewOffset NOTIFY currentViewOffsetChanged)

public:
    explicit World(QObject *parent = nullptr);
    ~World() override = default;

    QSize size() const;
    void setSize(const QSize &size);

    QSize boundingSize() const;
    void setBoundingSize(const QSize &boundingSize);

    QPoint currentViewOffset() const;
    void setCurrentViewOffset(const QPoint &currentViewOffset);

    QPoint currentPlayerPosition() const;
    Field *currentPlayerField();

    Map *map();
    Player *player();
    GameItems *gameItems();
    PlayerController *playerController();

    bool loaded() const;
    bool loading() const;

    Q_INVOKABLE void loadMap(const QString &fileName);

private:
    QSize m_size;
    QSize m_boundingSize;

    Map *m_map = nullptr;
    Player *m_player = nullptr;
    GameItems *m_gameItems = nullptr;
    PlayerController *m_playerController = nullptr;
    CollisionDetector *m_collisionDetector = nullptr;

    // View properties
    QPoint m_currentPlayerPosition;
    QPoint m_currentViewOffset;

    Field *m_currentPlayerField = nullptr;
    QList<Field *> m_fieldsInRange;
    GameItem *m_playerFocusItem = nullptr;

    // Map loading
    QFutureWatcher<void> *m_loadingWatcher = nullptr;
    bool m_loaded = false;
    bool m_loading = false;

    // Set methods
    void setCurrentPlayerPosition(const QPoint &currentPosition);
    void setCurrentPlayerField(Field *field);
    void setLoaded(bool loaded);
    void setLoading(bool loading);

    // Move methods
    void doPlayerMovement();

    // Helper methods
    Field *getFieldFromPosition(const QPointF position) const;
    QPointF adjustCollition(QPointF delta);
    void evaluateInRangeFields(const QPointF &playerPosition);

signals:
    void sizeChanged(const QSize &size);
    void boundingSizeChanged(const QSize &boundingSize);
    void loadingChanged(bool loading);
    void loadedChanged(bool loaded);

    void currentPlayerFieldChanged(Field *currentPlayerField);
    void currentPlayerPositionChanged(const QPoint &currentPlayerPosition);
    void currentViewOffsetChanged(const QPoint &currentViewOffset);

private slots:
    void onPlayerPositionChanged();
    void onLoadingFinished();

public slots:
    void tick();

};

#endif // WORLD_H
