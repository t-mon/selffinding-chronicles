#ifndef ENEMY_H
#define ENEMY_H

#include <QObject>

#include "character.h"

class Enemy : public Character
{
    Q_OBJECT
    Q_PROPERTY(int touchDamage READ touchDamage WRITE setTouchDamage NOTIFY touchDamageChanged)
    Q_PROPERTY(int shootDamage READ shootDamage WRITE setShootDamage NOTIFY shootDamageChanged)
    Q_PROPERTY(int hitDamage READ hitDamage WRITE setHitDamage NOTIFY hitDamageChanged)

public:
    explicit Enemy(QObject *parent = nullptr);

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    int touchDamage() const;
    void setTouchDamage(int touchDamage);

    int shootDamage() const;
    void setShootDamage(int shootDamage);

    int hitDamage() const;
    void setHitDamage(int hitDamage);

private:
    int m_touchDamage = 5;
    int m_shootDamage = 10;
    int m_hitDamage = 10;

signals:
    void touchDamageChanged(int touchDamage);
    void shootDamageChanged(int shootDamage);
    void hitDamageChanged(int hitDamage);

};

#endif // ENEMY_H
