#include "player.h"
#include "game.h"

#include <QtMath>
#include <QDebug>

Player::Player(QObject *parent) :
    QObject(parent),
    m_name("Player"),
    m_position(1.5, 1.5)
{

}

QString Player::name() const
{
    return m_name;
}

void Player::setName(const QString &name)
{
    if (m_name == name)
        return;

    qDebug() << "Player: name changed" << name;
    m_name = name;
    emit nameChanged(m_name);
}

QPointF Player::position() const
{
    return m_position;
}

void Player::setPosition(const QPointF position)
{
    if (m_position == position)
        return;

    qDebug() << "Player: position" << position;
    m_position = position;
    emit positionChanged(m_position);
    setMoving(true);
}

int Player::size() const
{
    return m_size;
}

void Player::setSize(const int &size)
{
    if (m_size == size)
        return;

    qDebug() << "Player: size changed" << size;
    m_size = size;
    emit sizeChanged(m_size);
}

qreal Player::angle() const
{
    return m_angle;
}

void Player::setAngle(const qreal &angle)
{
    if (m_angle == angle)
        return;

    qDebug() << "Player: angle" << angle;
    m_angle = angle;
    emit angleChanged(m_angle);
}

qreal Player::speed() const
{
    return m_speed;
}

void Player::setSpeed(const qreal speed)
{
    if (m_speed == speed)
        return;

    qDebug() << "Player: speed changed" << speed;
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

    qDebug() << "Player: movable changed" << movable;
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

    qDebug() << "Player" << m_name << (moving ? "moving" : "stopped");
    m_moving = moving;
    emit movingChanged(m_moving);
}

