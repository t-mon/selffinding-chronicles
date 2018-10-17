#ifndef CHARACTER_H
#define CHARACTER_H

#include <QObject>

#include "../gameobject.h"

// I know...a person is not an object, but...yeah :)

class Character : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(Gender gender READ gender NOTIFY genderChanged)
    Q_PROPERTY(int experience READ experience NOTIFY experienceChanged)
    Q_PROPERTY(int health READ health NOTIFY healthChanged)
    Q_PROPERTY(int mana READ mana NOTIFY manaChanged)
    Q_PROPERTY(int wisdom READ wisdom NOTIFY wisdomChanged)
    Q_PROPERTY(int strength READ strength NOTIFY strengthChanged)
    Q_PROPERTY(int stealth READ stealth NOTIFY stealthChanged)

public:
    enum Gender {
        Male,
        Female
    };
    Q_ENUM(Gender)

    Character(QObject *parent = nullptr);
    ~Character() override = default;

    Gender gender() const;
    void setGender(Gender gender);

    int experience() const;
    void setExperience(int experience);

    int health() const;
    void setHealth(int health);

    int mana() const;
    void setMana(int mana);

    int wisdom() const;
    void setWisdom(int wisdom);

    int strength() const;
    void setStrength(int strength);

    int stealth() const;
    void setStrealth(int stealth);

private:
    Gender m_gender = Male;
    int m_experience = 0;
    int m_health = 100;
    int m_mana = 5;
    int m_wisdom = 5;
    int m_strength = 5;
    int m_stealth = 5;

signals:
    void genderChanged(Gender gender);
    void experienceChanged(int experience);
    void healthChanged(int healt);
    void manaChanged(int mana);
    void wisdomChanged(int wisom);
    void strengthChanged(int strength);
    void stealthChanged(int stealth);

};

#endif // CHARACTER_H
