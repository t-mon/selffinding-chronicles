#include "plantitem.h"
#include "../debugcategories.h"

PlantItem::PlantItem(QObject *parent) :
    GameItem(parent)
{
    setInteraction(InteractionPick);
}

QString PlantItem::itemTypeName() const
{
    return tr("Plant");
}

GameItem::Type PlantItem::itemType() const
{
    return TypePlant;
}

void PlantItem::performInteraction()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "perform interaction" << m_interaction;
}

int PlantItem::healing() const
{
    return m_healing;
}

void PlantItem::setHealing(int healing)
{
    if (m_healing == healing)
        return;

    m_healing = healing;
    emit healingChanged(m_healing);
}

int PlantItem::mana() const
{
    return m_mana;
}

void PlantItem::setMana(int mana)
{
    if (m_mana == mana)
        return;

    m_mana = mana;
    emit manaChanged(m_mana);
}

int PlantItem::speed() const
{
    return m_speed;
}

void PlantItem::setSpeed(int speed)
{
    if (m_speed == speed)
        return;

    m_speed = speed;
    emit speedChanged(m_speed);
}
