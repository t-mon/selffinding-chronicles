#include "player.h"
#include "game.h"
#include "debugcategories.h"

#include <QSize>
#include <QtMath>

Player::Player(QObject *parent) :
    Character(parent)
{

}

qreal Player::speed() const
{
    return m_speed;
}

void Player::setSpeed(const qreal speed)
{
    qCDebug(dcPlayer()) << "Speed changed" << speed;
    m_speed = speed;
    emit speedChanged(m_speed);
}

bool Player::movable() const
{
    return m_movable;
}

void Player::setMovable(const bool &movable)
{
    if (m_movable == movable)
        return;

    qCDebug(dcPlayer()) << "Movable changed" << movable;
    m_movable = movable;
    emit movableChanged(m_movable);
}

bool Player::moving() const
{
    return m_moving;
}

void Player::setMoving(bool moving)
{
    if (m_moving == moving)
        return;

    qCDebug(dcPlayer()) << name() << (moving ? "moving" : "stopped");
    m_moving = moving;
    emit movingChanged(m_moving);
}

bool Player::running() const
{
    return m_running;
}

void Player::setRunning(bool running)
{
    if (m_running == running)
        return;

    m_running = running;
    emit runningChanged(m_running);

    qCDebug(dcPlayer()) << name() << (m_running ? "running" : "not running");
    if (m_running) {
        setSpeed(m_speed * 1.8);
    } else {
        setSpeed(0.03);
    }
}
