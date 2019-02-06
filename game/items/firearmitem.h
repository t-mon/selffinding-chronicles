#ifndef FIREARMITEM_H
#define FIREARMITEM_H

#include <QObject>

#include "gameitem.h"

class FirearmItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(int damage READ damage NOTIFY damageChanged)
    Q_PROPERTY(int range READ range NOTIFY rangeChanged)
    Q_PROPERTY(FirearmType firearmType READ firearmType NOTIFY firearmTypeChanged)

public:
    enum FirearmType {
        FirearmTypeBow,
        FirearmTypeCrossbow
    };
    Q_ENUM(FirearmType)

    explicit FirearmItem(QObject *parent = nullptr);
    ~FirearmItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    FirearmType firearmType() const;
    void setFirearmType(FirearmType firearmType);

    int damage() const;
    void setDamage(int damage);

    int range() const;
    void setRange(int range);

private:
    FirearmType m_firearmType = FirearmTypeBow;
    int m_damage = 0;
    int m_range = 0;

signals:
    void firearmTypeChanged(FirearmType firearmType);
    void damageChanged(int damage);
    void rangeChanged(int range);
};

QDebug operator<<(QDebug debug, FirearmItem *firearmItem);

#endif // FIREARMITEM_H
