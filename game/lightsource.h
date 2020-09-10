#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QObject>

#include <QImage>
#include <QColor>
#include <QDomDocument>
#include <QSvgRenderer>
#include <QQuickPaintedItem>

class LightSource : public QQuickPaintedItem
{
    Q_OBJECT
public:
    enum LightType {
        LightTypeSpotlight
    };
    Q_ENUM(LightType)

    explicit LightSource(QQuickItem *parent = nullptr);

    LightType lightType() const;
    void setLightType(LightType lightType);

    QColor color() const;
    void setColor(const QColor &color);

protected:
    virtual void paint(QPainter *painter);

private:
    QSvgRenderer *m_renderer = nullptr;
    LightType m_lightType = LightTypeSpotlight;
    QColor m_color;
    QDomDocument m_svgDom;

signals:
    void lightTypeChanged(LightType lightType);
    void colorChanged(const QColor &color);

private slots:
    void render();

};

#endif // LIGHTSOURCE_H
