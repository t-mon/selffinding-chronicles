#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPoint>

#include "gameobject.h"

class Player : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal auraRange READ auraRange WRITE setAuraRange NOTIFY auraRangeChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit Player(QObject *parent = nullptr);

    qreal angle() const;
    void setAngle(const qreal &angle);

    qreal auraRange() const;
    void setAuraRange(const qreal auraRange);

    qreal speed() const;
    void setSpeed(const qreal speed);

    bool movable() const;
    void setMovable(const bool &movable);

    bool moving() const;
    void setMoving(bool moving);

    bool running() const;
    void setRunning(bool running);

private:
    qreal m_angle = 0;
    qreal m_speed = 0.03;
    qreal m_auraRange = 3;
    bool m_movable = true;
    bool m_moving = false;
    bool m_running = false;

signals:
    void angleChanged(const qreal &angle);
    void speedChanged(const qreal &speed);
    void auraRangeChanged(const qreal &auraRange);
    void movableChanged(bool movable);
    void movingChanged(bool moving);
    void runningChanged(bool running);

};

#endif // PLAYER_H
