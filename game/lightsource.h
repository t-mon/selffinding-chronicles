#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include <QObject>
#include <QColor>

#include "gameobject.h"

class LightSource : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(LightType lightType READ lightType WRITE setLightType NOTIFY lightTypeChanged)
    Q_PROPERTY(LightBehaviour lightBehaviour READ lightBehaviour WRITE setLightBehaviour NOTIFY lightBehaviourChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(bool lightEnabled READ lightEnabled WRITE setLightEnabled NOTIFY lightEnabledChanged)

public:
    enum LightType {
        LightTypeSpotlight
    };
    Q_ENUM(LightType)

    enum LightBehaviour {
        LightBehaviourStill,
        LightBehaviourGlowing,
        LightBehaviourFlickering
    };
    Q_ENUM(LightBehaviour)


    explicit LightSource(QObject *parent = nullptr);

    QPointF offset() const;
    void setOffset(const QPointF &offset);

    LightType lightType() const;
    void setLightType(LightType lightType);

    LightBehaviour lightBehaviour() const;
    void setLightBehaviour(LightBehaviour lightBehaviour);

    QColor color() const;
    void setColor(const QColor &color);

    bool lightEnabled() const;
    void setLightEnabled(bool lightEnabled);

private:
    QPointF m_offset;
    LightType m_lightType = LightTypeSpotlight;
    LightBehaviour m_lightBehaviour = LightBehaviourGlowing;
    QColor m_color;
    bool m_lightEnabled = true;

signals:
    void offsetChanged(const QPointF &offset);
    void lightTypeChanged(LightType lightType);
    void lightBehaviourChanged(LightBehaviour lightBehaviour);
    void colorChanged(const QColor &color);
    void lightEnabledChanged(bool lightEnabled);

};

#endif // LIGHTSOURCE_H
