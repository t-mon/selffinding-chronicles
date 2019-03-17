#include "math.h"
#include "character.h"
#include "../debugcategories.h"

#include <QtMath>
#include <QVector2D>

Character::Character(QObject *parent):
    GameItem(parent)
{
    m_inventory = new GameItems(this);
    m_pathController = new PathController(this);
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

double Character::speed() const
{
    return m_speed;
}

void Character::setSpeed(double speed)
{
    qCDebug(dcCharacter()) << name() << "speed changed" << speed;
    m_speed = speed;
    emit speedChanged(m_speed);
}

bool Character::movable() const
{
    return m_movable;
}

void Character::setMovable(bool movable)
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
        setSpeed(1.2);
    } else {
        setSpeed(1.0);
    }
}

QList<Path *> Character::paths() const
{
    return m_paths;
}

void Character::setPaths(const QList<Path *> &paths)
{
    m_paths = paths;
}

Path *Character::currentPath() const
{
    return m_currentPath;
}

PathController *Character::pathController() const
{
    return m_pathController;
}

GameItems *Character::inventory() const
{
    return m_inventory;
}

void Character::lookToPoint(const QPointF &targetPoint)
{
    double angle = qAtan2(-(position().y() - targetPoint.y()), -(position().x() - targetPoint.x()));
    setAngle(angle);
}

Character::Armed Character::armed() const
{
    return m_armed;
}

void Character::setArmed(Character::Armed armed)
{
    if (m_armed == armed)
        return;

    qCDebug(dcCharacter()) << name() << "armed changed" << armed;
    m_armed = armed;
    emit armedChanged(m_armed);
}

WeaponItem *Character::weapon() const
{
    return m_weapon;
}

void Character::setWeapon(WeaponItem *weapon)
{
    if (m_weapon == weapon)
        return;

    qCDebug(dcCharacter()) << name() << "weapon changed" << weapon;
    m_weapon = weapon;
    emit weaponChanged(m_weapon);
    if (weapon) setArmed(ArmedWeapon);
}

FirearmItem *Character::firearm() const
{
    return m_firearm;
}

void Character::setFirearm(FirearmItem *firearm)
{
    if (m_firearm == firearm)
        return;

    qCDebug(dcCharacter()) << name() << "firearm changed" << firearm;

    m_firearm = firearm;
    emit firearmChanged(m_firearm);
    if (firearm) setArmed(ArmedFirearm);
}

QPointF Character::movementVector() const
{
    return m_movementVector;
}

void Character::setMovementVector(const QPointF &movementVector)
{
    if (m_movementVector == movementVector)
        return;

    m_movementVector = movementVector;
    emit movementVectorChanged(m_movementVector);
}

bool Character::isPlayer() const
{
    return m_isPlayer;
}

void Character::setIsPlayer(bool isPlayer)
{
    if (m_isPlayer == isPlayer)
        return;

    m_isPlayer = isPlayer;
    emit isPlayerChanged(m_isPlayer);
}

Character::Heading Character::heading() const
{
    return m_heading;
}

double Character::angle() const
{
    return m_angle;
}

void Character::setAngle(double angle)
{
    if (m_angle == angle)
        return;

    //qCDebug(dcCharacter()) << name() << "angle changed" << angle;
    m_angle = angle;
    emit angleChanged(m_angle);

    // FIXME: keep angble between +-180
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

void Character::setAuraRange(int auraRange)
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
    default:
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

bool Character::alive() const
{
    return m_alive;
}

int Character::health() const
{
    return m_health;
}

double Character::healthPercentage() const
{
    return 100.0 * m_health / m_healthMax;
}

void Character::setHealth(int health)
{
    if (m_health == health)
        return;

    qCDebug(dcCharacter()) << name() << "health changed" << health;
    if (health > m_healthMax) {
        m_health = m_healthMax;
        setAlive(true);
    } else if (health <= 0) {
        qCDebug(dcCharacter()) << name() << "killed!";
        m_health = 0;
        setAlive(false);
        emit killed();
    } else if (health > m_health) {
        qCDebug(dcCharacter()) << name() << "healed!";
        m_health = health;
        setAlive(true);
        emit healed();
    } else {
        qCDebug(dcCharacter()) << name() << "damaged!";
        m_health = health;
        setAlive(true);
        emit damaged();
    }

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

double Character::manaPercentage() const
{
    return 100.0 * m_mana / m_manaMax;
}

void Character::setMana(int mana)
{
    if (m_mana == mana)
        return;

    if (mana > m_manaMax) {
        m_mana = m_manaMax;
    } else if (mana <= 0) {
        m_mana = 0;
    } else {
        m_mana = mana;
    }

    qCDebug(dcCharacter()) << name() << "mana changed" << m_mana;
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

int Character::hitNumber() const
{
    return m_hitNumber;
}

void Character::setHitNumber(int hitNumber)
{
    if (m_hitNumber == hitNumber)
        return;

    qCDebug(dcCharacter()) << name() << "hit number changed" << hitNumber;
    m_hitNumber = hitNumber;
    emit hitNumberChanged(m_hitNumber);
    emit hit();
}

int Character::shootNumber() const
{
    return m_shootNumber;
}

void Character::setShootNumber(int shootNumber)
{
    if (m_shootNumber == shootNumber)
        return;

    qCDebug(dcCharacter()) << name() << "shoot number changed" << shootNumber;
    m_shootNumber = shootNumber;
    emit shootNumberChanged(m_shootNumber);
    emit shoot();
}

void Character::setHeading(Character::Heading heading)
{
    if (m_heading == heading)
        return;

    qCDebug(dcCharacter()) << name() << "heading changed" << heading;
    m_heading = heading;
    emit headingChanged(m_heading);
}

void Character::setAlive(bool alive)
{
    if (m_alive == alive)
        return;

    qCDebug(dcCharacter()) << name() << "alive changed" << alive;
    m_alive = alive;
    emit aliveChanged(m_alive);
}

void Character::onTick()
{
    // Check if we are following a path
    if (m_pathController->path()) {
        m_pathController->evaluatePosition(position());
        setMovementVector(m_pathController->movementVector());
        setAngle(m_pathController->angle());
    }
}


QDebug operator<<(QDebug debug, Character *character)
{
    debug.nospace() << "Character(" << character->itemTypeName();
    debug.nospace() << ", " << character->name();
    debug.nospace() << ", " << character->itemId();
    debug.nospace() << ", " << character->position();
    debug.nospace() << ", " << character->size();
    debug.nospace() << ")" << endl;

    foreach (GameItem *item, character->inventory()->gameItems()) {
        debug.nospace() << "    --> " << item->thread() << ", " << item << endl;
    }

    return debug.space();
}
