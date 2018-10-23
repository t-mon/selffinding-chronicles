#include "character.h"
#include "../debugcategories.h"

Character::Character(QObject *parent):
    GameItem(parent)
{
    m_auraCircleObject = new GameObject(this);
    m_auraCircleObject->setName("Aura");
    m_auraCircleObject->setShape(GameObject::ShapeCircle);

    m_inventory = new GameItems(this);
    m_inventoryProxy = new GameItemsProxy(this);
    m_inventoryProxy->setGameItems(m_inventory);
    m_inventoryProxy->setItemTypeFilter(GameItem::TypeWeapon);

    connect(this, &GameObject::positionChanged, this, &Character::onPositionChanged);
    updateAuraObject();
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

GameObject *Character::auraCircleObject() const
{
    return m_auraCircleObject;
}

GameItems *Character::inventory() const
{
    return m_inventory;
}

GameItemsProxy *Character::inventoryProxy() const
{
    return m_inventoryProxy;
}

qreal Character::angle() const
{
    return m_angle;
}

void Character::setAngle(const qreal &angle)
{
    m_angle = angle;
    emit angleChanged(m_angle);
}

int Character::auraRange() const
{
    return m_auraRange;
}

void Character::setAuraRange(const int auraRange)
{
    m_auraRange = auraRange;
    emit auraRangeChanged(m_auraRange);

    updateAuraObject();
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

int Character::healthMax() const
{
    return m_healthMax;
}

void Character::setHealthMax(int healthMax)
{
    if (m_healthMax == healthMax)
        return;

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


void Character::onPositionChanged(const QPointF newPosition)
{
    Q_UNUSED(newPosition)
    updateAuraObject();
}

void Character::updateAuraObject()
{
    m_auraCircleObject->setSize(QSize(auraRange() * 2 + 1, auraRange() * 2 + 1));
    m_auraCircleObject->setPosition(position() - QPointF(auraRange(), auraRange()));
}

