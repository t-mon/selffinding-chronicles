#include "svgitem.h"
#include "../debugcategories.h"

#include <QPainter>

SvgItem::SvgItem(QQuickItem *parent) :
    QQuickPaintedItem(parent)
{
    m_renderer = new QSvgRenderer(this);
}

QUrl SvgItem::source() const
{
    return m_source;
}

void SvgItem::setSource(const QUrl &source)
{
    if (m_source == source)
        return;

    m_source = source;
    emit sourceChanged(m_source);

    QFile file(m_source.toString());
    if (!file.open(QIODevice::ReadOnly)) {
        qCWarning(dcGame()) << "Could not open light svg" << file.fileName() << file.errorString();
        return;
    }

    m_svgDom.setContent(file.readAll());
    file.close();

    // TODO: set color and render
}

QColor SvgItem::color() const
{
    return m_color;
}

void SvgItem::setColor(const QColor &color)
{
    if (m_color == color)
        return;

    m_color = color;
    emit colorChanged(m_color);

    // TODO: update dom
}

void SvgItem::paint(QPainter *painter)
{
    painter->fillRect(0, 0, width(), height(), Qt::transparent);
    m_renderer->load(m_svgDom.toByteArray());
    m_renderer->render(painter);
}

void SvgItem::render()
{

}
