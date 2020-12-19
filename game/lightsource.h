#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QObject>
#include <QColor>

#include "gameobject.h"

class LightSource : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(LightType lightType READ lightType WRITE setLightType NOTIFY lightTypeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)

public:
    enum LightType {
        LightTypeSpotlight
    };
    Q_ENUM(LightType)

    explicit LightSource(QObject *parent = nullptr);

    LightType lightType() const;
    void setLightType(LightType lightType);

    QColor color() const;
    void setColor(const QColor &color);

    bool enabled() const;
    void setEnabled(bool enabled);

private:
    LightType m_lightType = LightTypeSpotlight;
    QColor m_color;
    bool m_enabled = false;

signals:
    void lightTypeChanged(LightType lightType);
    void colorChanged(const QColor &color);
    void enabledChanged(bool enabled);
};

#endif // LIGHTSOURCE_H
