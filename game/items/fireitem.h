#ifndef FIREITEM_H
#define FIREITEM_H

#include <QObject>

#include "gameitem.h"

class FireItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF offset READ offset WRITE setOffset NOTIFY offsetChanged)
    Q_PROPERTY(bool burning READ burning WRITE setBurning NOTIFY burningChanged)
    Q_PROPERTY(double fireAngle READ fireAngle WRITE setFireAngle NOTIFY fireAngleChanged)
    Q_PROPERTY(double fireAngleVariation READ fireAngleVariation WRITE setFireAngleVariation NOTIFY fireAngleVariationChanged)
    Q_PROPERTY(double fireMagnitude READ fireMagnitude WRITE setFireMagnitude NOTIFY fireMagnitudeChanged)

public:
    explicit FireItem(QObject *parent = nullptr);

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    QPointF offset() const;
    void setOffset(const QPointF &offset);

    bool burning() const;
    void setBurning(bool burning);

    double fireAngle() const;
    void setFireAngle(double fireAngle);

    double fireAngleVariation() const;
    void setFireAngleVariation(double fireAngleVariation);

    double fireMagnitude() const;
    void setFireMagnitude(double fireMagnitude);

signals:
    void offsetChanged(const QPointF &offset);
    void burningChanged(bool burning);
    void fireAngleChanged(double fireAngle);
    void fireAngleVariationChanged(double fireAngleVariation);
    void fireMagnitudeChanged(double fireMagnitude);

private:
    QPointF m_offset;
    bool m_burning = false;
    double m_fireAngle = 270;
    double m_fireAngleVariation = 22;
    double m_fireMagnitude = 20;
};

#endif // FIREITEM_H
