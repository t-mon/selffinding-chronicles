#include "math.h"
#include "character.h"
#include "../debugcategories.h"

Character::Character(QObject *parent):
    GameItem(parent)
{
    m_inventory = new GameItems(this);
    m_inventoryProxy = new GameItemsProxy(this);
    m_inventoryProxy->setGameItems(m_inventory);
    m_inventoryProxy->setItemTypeFilter(GameItem::TypeWeapon);
}

QString Character::itemTypeName() const
{
    return tr("Character");
}

GameItem::Type Character::itemType() const
{
    return GameItem::TypeCharacter;
}

void Character::performInteraction()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "perform interaction" << m_interaction;
}

qreal Character::speed() const
{
    return m_speed;
}

void Character::setSpeed(const qreal speed)
{
    qCDebug(dcCharacter()) << name() << "speed changed" << speed;
    m_speed = speed;
    emit speedChanged(m_speed);
}

bool Character::movable() const
{
    return m_movable;
}

void Character::setMovable(const bool &movable)
{
    if (m_movable == movable)
        return;

    qCDebug(dcCharacter()) << name() << "movable changed" << movable;
    m_movable = movable;
    emit movableChanged(m_movable);
}

bool Character::moving() const
{
    return m_moving;
}

void Character::setMoving(bool moving)
{
    if (m_moving == moving)
        return;

    qCDebug(dcCharacter()) << name() << (moving ? "moving" : "stopped");
    m_moving = moving;
    emit movingChanged(m_moving);
}

bool Character::running() const
{
    return m_running;
}

void Character::setRunning(bool running)
{
    if (m_running == running)
        return;

    qCDebug(dcCharacter()) << name() << (running ? "running" : "not running");
    m_running = running;
    emit runningChanged(m_running);

    if (m_running) {
        setSpeed(0.3);
    } else {
        setSpeed(0.2);
    }
}

GameItems *Character::inventory() const
{
    return m_inventory;
}

GameItemsProxy *Character::inventoryProxy() const
{
    return m_inventoryProxy;
}

Character::Heading Character::heading() const
{
    return m_heading;
}

qreal Character::angle() const
{
    return m_angle;
}

void Character::setAngle(const qreal &angle)
{
    if (m_angle == angle)
        return;

    qCDebug(dcCharacter()) << name() << "angle changed" << angle;
    m_angle = angle;
    emit angleChanged(m_angle);

    if (angle <= -M_PI_2 || angle >= M_PI_2) {
        setHeading(HeadingLeft);
    } else {
        setHeading(HeadingRight);
    }
}

int Character::auraRange() const
{
    return m_auraRange;
}

void Character::setAuraRange(const int auraRange)
{
    m_auraRange = auraRange;
    emit auraRangeChanged(m_auraRange);
}

Character::Gender Character::gender() const
{
    return m_gender;
}

void Character::setGender(Character::Gender gender)
{
    if (m_gender == gender)
        return;

    qCDebug(dcCharacter()) << name() << "gender changed" << gender;
    m_gender = gender;
    emit genderChanged(m_gender);
}

Character::Role Character::role() const
{
    return m_role;
}

void Character::setRole(Character::Role role)
{
    if (m_role == role)
        return;

    qCDebug(dcCharacter()) << name() << "role changed" << role;
    m_role = role;
    emit roleChanged(role);

    switch (m_role) {
    case Player:
    case Statist:
        setInteraction(InteractionNone);
        break;
    case Friend:
    case Professor:
    case Magician:
    case Warrior:
    case Dealer:
        setInteraction(InteractionTalk);
        break;
    case Enemy:
        setInteraction(InteractionAttack);
        break;
    }
}

int Character::experience() const
{
    return m_experience;
}

void Character::setExperience(int experience)
{
    if (m_experience == experience)
        return;

    qCDebug(dcCharacter()) << name() << "experience changed" << experience;
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

    qCDebug(dcCharacter()) << name() << "health changed" << health;
    m_health = health;
    emit healthChanged(m_health);
}

int Character::healthMax() const
{
    return m_healthMax;
}

void Character::setHealthMax(int healthMax)
{
    if (m_healthMax == healthMax)
        return;

    qCDebug(dcCharacter()) << name() << "health max changed" << healthMax;
    m_healthMax = healthMax;
    emit healthMaxChanged(m_healthMax);
}

int Character::mana() const
{
    return m_mana;
}

void Character::setMana(int mana)
{
    if (m_mana == mana)
        return;

    qCDebug(dcCharacter()) << name() << "mana changed" << mana;
    m_mana = mana;
    emit manaChanged(m_mana);
}

int Character::manaMax() const
{
    return m_manaMax;
}

void Character::setManaMax(int manaMax)
{
    if (m_manaMax == manaMax)
        return;

    qCDebug(dcCharacter()) << name() << "mana max changed" << manaMax;
    m_manaMax = manaMax;
    emit manaMaxChanged(m_manaMax);
}

int Character::wisdom() const
{
    return m_wisdom;
}

void Character::setWisdom(int wisdom)
{
    if (m_wisdom == wisdom)
        return;

    qCDebug(dcCharacter()) << name() << "wisdom changed" << wisdom;
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

    qCDebug(dcCharacter()) << name() << "strength changed" << strength;
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

    qCDebug(dcCharacter()) << name() << "stealth changed" << stealth;
    m_stealth = stealth;
    emit stealthChanged(m_stealth);
}


void Character::setHeading(Character::Heading heading)
{
    if (m_heading == heading)
        return;

    qCDebug(dcCharacter()) << name() << "heading changed" << heading;
    m_heading = heading;
    emit headingChanged(m_heading);
}

