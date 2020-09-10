#include "lightsource.h"
#include "debugcategories.h"

#include <QPainter>

LightSource::LightSource(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    m_renderer = new QSvgRenderer(this);
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

    switch (m_lightType) {
    case LightTypeSpotlight:
        QFile file("qrc:/lights/spotlight.svg");
        if (!file.open(QIODevice::ReadOnly)) {
            qCWarning(dcGame()) << "Could not open light svg" << file.fileName() << file.errorString();
            return;
        }

        m_svgDom.setContent(file.readAll());
        file.close();
        break;
    }
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

    // Update svg content
}

void LightSource::paint(QPainter *painter)
{
    painter->fillRect(0, 0, width(), height(), Qt::transparent);
    m_renderer->load(m_svgDom.toByteArray());
    m_renderer->render(painter);
}

void LightSource::render()
{

}
