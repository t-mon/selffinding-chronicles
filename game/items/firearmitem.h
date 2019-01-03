#ifndef FIREARMITEM_H
#define FIREARMITEM_H

#include <QObject>

#include "gameitem.h"

class FirearmItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(int damage READ damage NOTIFY damageChanged)

public:
    explicit FirearmItem(QObject *parent = nullptr);
    ~FirearmItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    int damage() const;
    void setDamage(int damage);

private:
    int m_damage = 0;

signals:
    void damageChanged(int damage);

};

#endif // FIREARMITEM_H
