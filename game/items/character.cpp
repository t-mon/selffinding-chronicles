#include "character.h"

Character::Character(QObject *parent):
    GameObject(parent)
{

}

Character::Gender Character::gender() const
{
    return m_gender;
}

void Character::setGender(Character::Gender gender)
{
    if (m_gender == gender)
        return;

    m_gender = gender;
    emit genderChanged(m_gender);
}

int Character::experience() const
{
    return m_experience;
}

void Character::setExperience(int experience)
{
    if (m_experience == experience)
        return;

    m_experience = experience;
    emit experienceChanged(m_experience);
}

int Character::health() const
{
    return m_health;
}

void Character::setHealth(int health)
{
    if (m_health == health)
        return;

    m_health = health;
    emit healthChanged(m_health);
}

int Character::mana() const
{
    return m_mana;
}

void Character::setMana(int mana)
{
    if (m_mana == mana)
        return;

    m_mana = mana;
    emit manaChanged(m_mana);
}

int Character::wisdom() const
{
    return m_wisdom;
}

void Character::setWisdom(int wisdom)
{
    if (m_wisdom == wisdom)
        return;

    m_wisdom = wisdom;
    emit wisdomChanged(m_wisdom);
}

int Character::strength() const
{
    return m_strength;
}

void Character::setStrength(int strength)
{
    if (m_strength == strength)
        return;

    m_strength = strength;
    emit strengthChanged(m_strength);
}

int Character::stealth() const
{
    return m_stealth;
}

void Character::setStrealth(int stealth)
{
    if (m_stealth == stealth)
        return;

    m_stealth = stealth;
    emit stealthChanged(m_stealth);
}
