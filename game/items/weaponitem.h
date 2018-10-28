#ifndef WEAPONITEM_H
#define WEAPONITEM_H

#include <QObject>

#include "gameitem.h"

class WeaponItem : public GameItem
{
    Q_OBJECT

public:
    explicit WeaponItem(QObject *parent = nullptr);
    ~WeaponItem() override = default;

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

#endif // WEAPONITEM_H
