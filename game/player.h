#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPoint>

#include "gameobject.h"
#include "items/gameitemsproxy.h"

class Player : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(GameObject *auraCircleObject READ auraCircleObject CONSTANT)
    Q_PROPERTY(GameItems *inventory READ inventory CONSTANT)
    Q_PROPERTY(GameItemsProxy *inventoryProxy READ inventoryProxy CONSTANT)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal auraRange READ auraRange WRITE setAuraRange NOTIFY auraRangeChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

    Q_PROPERTY(int health READ health NOTIFY healthChanged)
    Q_PROPERTY(int mana READ mana NOTIFY manaChanged)
    Q_PROPERTY(int wisdom READ wisdom NOTIFY wisdomChanged)

public:
    explicit Player(QObject *parent = nullptr);

    GameObject *auraCircleObject() const;
    GameItems *inventory() const;
    GameItemsProxy *inventoryProxy() const;

    qreal angle() const;
    void setAngle(const qreal &angle);

    int auraRange() const;
    void setAuraRange(const int auraRange);

    qreal speed() const;
    void setSpeed(const qreal speed);

    bool movable() const;
    void setMovable(const bool &movable);

    bool moving() const;
    void setMoving(bool moving);

    bool running() const;
    void setRunning(bool running);

    int health() const;
    void setHealth(int health);

    int mana() const;
    void setMana(int mana);

    int wisdom() const;
    void setWisdom(int wisdom);

private:
    GameObject *m_auraCircleObject = nullptr;
    GameItems *m_inventory = nullptr;
    GameItemsProxy *m_inventoryProxy = nullptr;
    qreal m_angle = 0;
    qreal m_speed = 0.03;
    int m_auraRange = 3;
    bool m_movable = true;
    bool m_moving = false;
    bool m_running = false;

    int m_health = 100;
    int m_mana = 30;
    int m_wisdom = 30;

private slots:
    void onPositionChanged(const QPointF newPosition);
    void updateAuraObject();

signals:
    void angleChanged(const qreal &angle);
    void speedChanged(const qreal &speed);
    void auraRangeChanged(const int &auraRange);
    void movableChanged(bool movable);
    void movingChanged(bool moving);
    void runningChanged(bool running);

    void healthChanged(int healt);
    void manaChanged(int mana);
    void wisdomChanged(int wisom);

};

#endif // PLAYER_H
