#include "fireitem.h"

FireItem::FireItem(QObject *parent) : GameItem(parent)
{
    setInteraction(InteractionIgnite);
}

QString FireItem::itemTypeName() const
{
    return "Fire";
}

GameItem::Type FireItem::itemType() const
{
    return GameItem::TypeFire;
}

void FireItem::performInteraction()
{
    switch (m_interaction) {
    case InteractionIgnite:
        setBurning(true);
        setInteraction(InteractionExtinguish);
        break;
    case InteractionExtinguish:
        setBurning(false);
        setInteraction(InteractionIgnite);
        break;
    default:
        break;
    }

}

QPointF FireItem::offset() const
{
    return m_offset;
}

void FireItem::setOffset(const QPointF &offset)
{
    if (m_offset == offset)
        return;

    m_offset = offset;
    emit offsetChanged(m_offset);
}

bool FireItem::burning() const
{
    return m_burning;
}

void FireItem::setBurning(bool burning)
{
    if (m_burning == burning)
        return;

    m_burning = burning;
    emit burningChanged(m_burning);

    if (m_burning) {
        setInteraction(InteractionExtinguish);
    } else {
        setInteraction(InteractionIgnite);
    }

    if (lightSource()) {
        lightSource()->setLightEnabled(m_burning);
    }
}

double FireItem::fireAngle() const
{
    return m_fireAngle;
}

void FireItem::setFireAngle(double fireAngle)
{
    if (m_fireAngle == fireAngle)
        return;

    m_fireAngle = fireAngle;
    emit fireAngleChanged(m_fireAngle);
}

double FireItem::fireAngleVariation() const
{
    return m_fireAngleVariation;
}

void FireItem::setFireAngleVariation(double fireAngleVariation)
{
    if (m_fireAngleVariation == fireAngleVariation)
        return;

    m_fireAngleVariation = fireAngleVariation;
    emit fireAngleVariationChanged(m_fireAngleVariation);
}

double FireItem::fireMagnitude() const
{
    return m_fireMagnitude;
}

void FireItem::setFireMagnitude(double fireMagnitude)
{
    if (m_fireMagnitude == fireMagnitude)
        return;

    m_fireMagnitude = fireMagnitude;
    emit fireMagnitudeChanged(m_fireMagnitude);
}
