#include "weaponitem.h"
#include "../debugcategories.h"

WeaponItem::WeaponItem(QObject *parent):
    GameItem(parent)
{
    setInteraction(InteractionPick);
}

QString WeaponItem::itemTypeName() const
{
    return tr("Weapon");
}

GameItem::Type WeaponItem::itemType() const
{
    return TypeWeapon;
}

void WeaponItem::performInteraction()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "perform interaction" << m_interaction;
}

int WeaponItem::damage() const
{
    return m_damage;
}

void WeaponItem::setDamage(int damage)
{
    if (m_damage == damage)
        return;

    m_damage = damage;
    emit damageChanged(m_damage);
}
