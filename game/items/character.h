#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>

#include "weaponitem.h"
#include "firearmitem.h"
#include "gameitemsproxy.h"
#include "../gameobject.h"
#include "../pathfollowingcontroller.h"

// I know...a person is not an item, but...yeah :)

class Character : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(GameItems *inventory READ inventory CONSTANT)

    Q_PROPERTY(Armed armed READ armed WRITE setArmed NOTIFY armedChanged)
    Q_PROPERTY(WeaponItem *weapon READ weapon WRITE setWeapon NOTIFY weaponChanged)
    Q_PROPERTY(FirearmItem *firearm READ firearm WRITE setFirearm NOTIFY firearmChanged)

    Q_PROPERTY(bool isPlayer READ isPlayer NOTIFY isPlayerChanged)
    Q_PROPERTY(Heading heading READ heading NOTIFY headingChanged)
    Q_PROPERTY(QPointF movementVector READ movementVector WRITE setMovementVector NOTIFY movementVectorChanged)
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(qreal auraRange READ auraRange WRITE setAuraRange NOTIFY auraRangeChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(bool running READ running NOTIFY runningChanged)

    Q_PROPERTY(Gender gender READ gender NOTIFY genderChanged)
    Q_PROPERTY(Role role READ role NOTIFY roleChanged)
    Q_PROPERTY(int experience READ experience NOTIFY experienceChanged)
    Q_PROPERTY(bool alive READ alive NOTIFY healthChanged)
    Q_PROPERTY(int health READ health NOTIFY healthChanged)
    Q_PROPERTY(int healthMax READ healthMax NOTIFY healthMaxChanged)
    Q_PROPERTY(double healthPercentage READ healthPercentage NOTIFY healthChanged)
    Q_PROPERTY(int mana READ mana NOTIFY manaChanged)
    Q_PROPERTY(int manaMax READ manaMax NOTIFY manaMaxChanged)
    Q_PROPERTY(double manaPercentage READ manaPercentage NOTIFY manaChanged)
    Q_PROPERTY(int wisdom READ wisdom NOTIFY wisdomChanged)
    Q_PROPERTY(int strength READ strength NOTIFY strengthChanged)
    Q_PROPERTY(int stealth READ stealth NOTIFY stealthChanged)

    Q_PROPERTY(int hitNumber READ hitNumber WRITE setHitNumber NOTIFY hitNumberChanged)
    Q_PROPERTY(int shootNumber READ shootNumber WRITE setShootNumber NOTIFY shootNumberChanged)

public:
    enum Gender {
        Male,
        Female
    };
    Q_ENUM(Gender)

    enum Role {
        None,
        Player,
        Statist,
        Friend,
        Enemy,
        Professor,
        Magician,
        Warrior,
        Dealer
    };
    Q_ENUM(Role)

    enum Heading {
        HeadingLeft,
        HeadingRight
    };
    Q_ENUM(Heading)

    enum Armed {
        ArmedNone,
        ArmedWeapon,
        ArmedFirearm
    };
    Q_ENUM(Armed)

    Character(QObject *parent = nullptr);
    ~Character() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    GameItems *inventory() const;

    Armed armed() const;
    void setArmed(Armed armed);

    WeaponItem *weapon() const;
    void setWeapon(WeaponItem *weapon);

    FirearmItem *firearm() const;
    void setFirearm(FirearmItem *firearm);

    QPointF movementVector() const;
    void setMovementVector(const QPointF &movementVector);

    bool isPlayer() const;
    void setIsPlayer(bool isPlayer);

    // Movement
    Heading heading() const;

    qreal angle() const;
    void setAngle(const qreal &angle);

    qreal speed() const;
    void setSpeed(const qreal speed);

    int auraRange() const;
    void setAuraRange(const int auraRange);

    bool movable() const;
    void setMovable(const bool &movable);

    bool moving() const;
    void setMoving(bool moving);

    bool running() const;
    void setRunning(bool running);

    QList<Path *> paths() const;
    void setPaths(const QList<Path *> &paths);

    // Properties
    Gender gender() const;
    void setGender(Gender gender);

    Role role() const;
    void setRole(Role role);

    int experience() const;
    void setExperience(int experience);

    bool alive() const;

    int health() const;
    double healthPercentage() const;
    void setHealth(int health);

    int healthMax() const;
    void setHealthMax(int healthMax);

    int mana() const;
    double manaPercentage() const;
    void setMana(int mana);

    int manaMax() const;
    void setManaMax(int manaMax);

    int wisdom() const;
    void setWisdom(int wisdom);

    int strength() const;
    void setStrength(int strength);

    int stealth() const;
    void setStrealth(int stealth);

    int hitNumber() const;
    void setHitNumber(int hitNumber);

    int shootNumber() const;
    void setShootNumber(int shootNumber);

private:
    GameItems *m_inventory = nullptr;
    Armed m_armed = ArmedNone;
    WeaponItem *m_weapon = nullptr;
    FirearmItem *m_firearm = nullptr;

    // Movement
    QPointF m_movementVector = QPointF(0, 0);
    bool m_isPlayer = false;
    Heading m_heading = HeadingRight;
    int m_auraRange = 3;
    qreal m_angle = 0;
    qreal m_speed = 1;
    bool m_movable = true;
    bool m_moving = false;
    bool m_running = false;
    QList<Path *> m_paths;

    // Properties
    Gender m_gender = Male;
    Role m_role = Player;
    int m_experience = 0;

    bool m_alive = true;
    int m_health = 100;
    int m_healthMax = 100;

    int m_mana = 10;
    int m_manaMax = 10;

    int m_wisdom = 5;
    int m_strength = 5;
    int m_stealth = 5;

    int m_hitNumber = 0;
    int m_shootNumber = 0;

    void setHeading(Character::Heading heading);
    void setAlive(bool alive);

signals:
    void armedChanged(Armed armed);
    void weaponChanged(WeaponItem *weapon);
    void firearmChanged(FirearmItem *firearm);

    void movementVectorChanged(const QPointF &movementVector);
    void isPlayerChanged(bool isPlayer);
    void headingChanged(Heading heading);
    void angleChanged(const qreal &angle);
    void auraRangeChanged(const int &auraRange);
    void speedChanged(const qreal &speed);
    void movableChanged(bool movable);
    void movingChanged(bool moving);
    void runningChanged(bool running);

    void genderChanged(Gender gender);
    void roleChanged(Role role);
    void experienceChanged(int experience);

    void aliveChanged(bool alive);
    void healthChanged(int healt);
    void healthMaxChanged(int healtMax);

    void manaChanged(int mana);
    void manaMaxChanged(int manaMax);

    void wisdomChanged(int wisom);
    void strengthChanged(int strength);
    void stealthChanged(int stealth);

    void hitNumberChanged(int hitNumber);
    void shootNumberChanged(int shootNumber);

    void damaged();
    void healed();
    void killed();
    void hit();
    void shoot();

};

QDebug operator<<(QDebug debug, Character *character);


#endif // CHARACTER_H
