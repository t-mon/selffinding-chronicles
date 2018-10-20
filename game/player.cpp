#include "player.h"
#include "game.h"
#include "debugcategories.h"

#include <QSize>
#include <QtMath>

Player::Player(QObject *parent) :
    Character(parent)
{
    m_auraCircleObject = new GameObject(this);
    m_auraCircleObject->setName("Aura");
    m_auraCircleObject->setShape(GameObject::ShapeCircle);

    m_inventory = new GameItems(this);
    m_inventoryProxy = new GameItemsProxy(this);
    m_inventoryProxy->setGameItems(m_inventory);
    m_inventoryProxy->setItemTypeFilter(GameItem::TypeWeapon);

    connect(this, &GameObject::positionChanged, this, &Player::onPositionChanged);
    updateAuraObject();
}

GameObject *Player::auraCircleObject() const
{
    return m_auraCircleObject;
}

GameItems *Player::inventory() const
{
    return m_inventory;
}

GameItemsProxy *Player::inventoryProxy() const
{
    return m_inventoryProxy;
}

qreal Player::angle() const
{
    return m_angle;
}

void Player::setAngle(const qreal &angle)
{
    //qCDebug(dcPlayer()) << "angle" << angle;
    m_angle = angle;
    emit angleChanged(m_angle);
}

int Player::auraRange() const
{
    return m_auraRange;
}

void Player::setAuraRange(const int auraRange)
{
    qCDebug(dcPlayer()) << "Aura range changed" << auraRange;
    m_auraRange = auraRange;
    emit auraRangeChanged(m_auraRange);

    updateAuraObject();
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

void Player::onPositionChanged(const QPointF newPosition)
{
    Q_UNUSED(newPosition)
    updateAuraObject();
}

void Player::updateAuraObject()
{
    m_auraCircleObject->setSize(QSize(auraRange() * 2 + 1, auraRange() * 2 + 1));
    m_auraCircleObject->setPosition(position() - QPointF(auraRange(), auraRange()));
}

