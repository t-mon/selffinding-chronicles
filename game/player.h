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
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

public:
    explicit Player(QObject *parent = nullptr);
    ~Player() override = default;

    qreal speed() const;
    void setSpeed(const qreal speed);

    bool movable() const;
    void setMovable(const bool &movable);

    bool moving() const;
    void setMoving(bool moving);

    bool running() const;
    void setRunning(bool running);

private:
    qreal m_speed = 0.03;
    bool m_movable = true;
    bool m_moving = false;
    bool m_running = false;

signals:
    void speedChanged(const qreal &speed);
    void movableChanged(bool movable);
    void movingChanged(bool moving);
    void runningChanged(bool running);


};

#endif // PLAYER_H
