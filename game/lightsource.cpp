#include "lightsource.h"
#include "debugcategories.h"


LightSource::LightSource(QObject *parent) :
    GameObject(parent)
{

}

QPointF LightSource::offset() const
{
    return m_offset;
}

void LightSource::setOffset(const QPointF &offset)
{
    if (m_offset == offset)
        return;

    m_offset = offset;
    emit offsetChanged(m_offset);
}

LightSource::LightType LightSource::lightType() const
{
    return m_lightType;
}

void LightSource::setLightType(LightSource::LightType lightType)
{
    if (m_lightType == lightType)
        return;

    m_lightType = lightType;
    emit lightTypeChanged(m_lightType);
}

LightSource::LightBehaviour LightSource::lightBehaviour() const
{
    return m_lightBehaviour;
}

void LightSource::setLightBehaviour(LightSource::LightBehaviour lightBehaviour)
{
    if (m_lightBehaviour == lightBehaviour)
        return;

    m_lightBehaviour = lightBehaviour;
    emit lightBehaviourChanged(m_lightBehaviour);
}

QColor LightSource::color() const
{
    return m_color;
}

void LightSource::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);
}

bool LightSource::lightEnabled() const
{
    return m_lightEnabled;
}

void LightSource::setLightEnabled(bool lightEnabled)
{
    if (m_lightEnabled == lightEnabled)
        return;

    qCDebug(dcGameObject()) << "Light" << name() << (lightEnabled ? "enabled" : "disabled");
    m_lightEnabled = lightEnabled;
    emit lightEnabledChanged(m_lightEnabled);
}
