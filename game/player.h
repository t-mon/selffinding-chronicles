#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPoint>

#include "gameobject.h"
#include "items/character.h"
#include "items/gameitemsproxy.h"

class Player : public Character
{
    Q_OBJECT
    Q_PROPERTY(GameObject *auraCircleObject READ auraCircleObject CONSTANT)
    Q_PROPERTY(GameItems *inventory READ inventory CONSTANT)
    Q_PROPERTY(GameItemsProxy *inventoryProxy READ inventoryProxy CONSTANT)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal auraRange READ auraRange WRITE setAuraRange NOTIFY auraRangeChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit Player(QObject *parent = nullptr);
    ~Player() override = default;

    GameObject *auraCircleObject() const;
    GameItems *inventory() const;
    GameItemsProxy *inventoryProxy() const;

    qreal angle() const;
    void setAngle(const qreal &angle);

    int auraRange() const;
    void setAuraRange(const int auraRange);

    qreal speed() const;
    void setSpeed(const qreal speed);

    bool movable() const;
    void setMovable(const bool &movable);

    bool moving() const;
    void setMoving(bool moving);

    bool running() const;
    void setRunning(bool running);

private:
    GameObject *m_auraCircleObject = nullptr;
    GameItems *m_inventory = nullptr;
    GameItemsProxy *m_inventoryProxy = nullptr;
    qreal m_angle = 0;
    qreal m_speed = 0.03;
    int m_auraRange = 3;
    bool m_movable = true;
    bool m_moving = false;
    bool m_running = false;


private slots:
    void onPositionChanged(const QPointF newPosition);
    void updateAuraObject();

signals:
    void angleChanged(const qreal &angle);
    void speedChanged(const qreal &speed);
    void auraRangeChanged(const int &auraRange);
    void movableChanged(bool movable);
    void movingChanged(bool moving);
    void runningChanged(bool running);


};

#endif // PLAYER_H
