#include "bullet.h"

Bullet::Bullet(QObject *parent) :
    QObject(parent)
{

}

Bullet::~Bullet()
{

}

Character *Bullet::shooter() const
{
    return m_shooter;
}

void Bullet::setShooter(Character *shooter)
{
    if (m_shooter == shooter)
        return;

    m_shooter = shooter;
    emit shooterChanged(m_shooter);
}

int Bullet::damage() const
{
    return m_damage;
}

void Bullet::setDamage(int damage)
{
    if (m_damage == damage)
        return;

    m_damage = damage;
    emit damageChanged(m_damage);
}
