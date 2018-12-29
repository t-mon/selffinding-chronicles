#include "enemy.h"
#include "../debugcategories.h"

Enemy::Enemy(QObject *parent) : Character(parent)
{
    setRole(Role::Enemy);
}

QString Enemy::itemTypeName() const
{
    return tr("Enemy");
}

GameItem::Type Enemy::itemType() const
{
    return GameItem::TypeEnemy;
}

void Enemy::performInteraction()
{
    qCDebug(dcItem()) << name() << "Perform interaction" << this;
}

int Enemy::touchDamage() const
{
    return m_touchDamage;
}

void Enemy::setTouchDamage(int touchDamage)
{
    if (m_touchDamage == touchDamage)
        return;

    m_touchDamage = touchDamage;
    emit touchDamageChanged(m_touchDamage);
}

int Enemy::shootDamage() const
{
    return m_shootDamage;
}

void Enemy::setShootDamage(int shootDamage)
{
    if (m_shootDamage == shootDamage)
        return;

    m_shootDamage = shootDamage;
    emit shootDamageChanged(m_shootDamage);
}

int Enemy::hitDamage() const
{
    return m_hitDamage;
}

void Enemy::setHitDamage(int hitDamage)
{
    if (m_hitDamage == hitDamage)
        return;

    m_hitDamage = hitDamage;
    emit hitDamageChanged(m_hitDamage);
}
