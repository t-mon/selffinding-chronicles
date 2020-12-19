#include "lightsource.h"
#include "debugcategories.h"


LightSource::LightSource(QObject *parent) :
    GameObject(parent)
{

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

bool LightSource::enabled() const
{
    return m_enabled;
}

void LightSource::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}
