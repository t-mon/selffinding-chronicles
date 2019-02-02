#include "firearmitem.h"
#include "../debugcategories.h"

FirearmItem::FirearmItem(QObject *parent) :
    GameItem(parent)
{
    setInteraction(InteractionPick);
}

QString FirearmItem::itemTypeName() const
{
    return tr("Firearm");
}

GameItem::Type FirearmItem::itemType() const
{
    return TypeFirearm;
}

void FirearmItem::performInteraction()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "perform interaction" << m_interaction;
}

int FirearmItem::damage() const
{
    return m_damage;
}

void FirearmItem::setDamage(int damage)
{
    if (m_damage == damage)
        return;

    m_damage = damage;
    emit damageChanged(m_damage);
}

int FirearmItem::range() const
{
    return m_range;
}

void FirearmItem::setRange(int range)
{
    if (m_range == range)
        return;

    m_range = range;
    emit rangeChanged(m_range);
}


QDebug operator<<(QDebug debug, FirearmItem *firearmItem)
{
    debug.nospace() << "Firearm("<< firearmItem->name();
    debug.nospace() << ", " << firearmItem->itemId();
    debug.nospace() << ", price: " << firearmItem->price();
    debug.nospace() << ", damage: " << firearmItem->damage();
    debug.nospace() << ", range: " << firearmItem->range();
    debug.nospace() << ")";
    return debug.space();
}
