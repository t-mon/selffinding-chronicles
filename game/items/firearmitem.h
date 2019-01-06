#ifndef FIREARMITEM_H
#define FIREARMITEM_H

#include <QObject>

#include "gameitem.h"

class FirearmItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(int damage READ damage NOTIFY damageChanged)
    Q_PROPERTY(int range READ range NOTIFY rangeChanged)

public:
    explicit FirearmItem(QObject *parent = nullptr);
    ~FirearmItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    int damage() const;
    void setDamage(int damage);

    int range() const;
    void setRange(int range);

private:
    int m_damage = 0;
    int m_range = 0;

signals:
    void damageChanged(int damage);
    void rangeChanged(int range);
};

#endif // FIREARMITEM_H
