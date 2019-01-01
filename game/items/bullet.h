#ifndef BULLET_H
#define BULLET_H

#include <QObject>

#include "character.h"

class Bullet: public QObject
{
    Q_OBJECT
    Q_PROPERTY(Character *shooter READ shooter WRITE setShooter NOTIFY shooterChanged)
    Q_PROPERTY(int damage READ damage WRITE setDamage NOTIFY damageChanged)

public:
    Bullet(QObject *parent = nullptr);
    ~Bullet();

    Character *shooter() const;
    void setShooter(Character *shooter);

    int damage() const;
    void setDamage(int damage);

private:
    Character *m_shooter = nullptr;
    int m_damage = 0;

signals:
    void shooterChanged(Character *shooter);
    void damageChanged(int damage);

};

#endif // BULLET_H
