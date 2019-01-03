#include "gameobject.h"
#include "dataloader.h"
#include "debugcategories.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCryptographicHash>

QList<GameItem *> DataLoader::loadGameItems(const QVariantList &itemsList)
{
    QList<GameItem *> gameItems;

    foreach (const QVariant &itemVariant, itemsList) {
        QVariantMap itemMap = itemVariant.toMap();
        QVariantMap mapData = loadJsonData(itemMap.value("data").toString());

        if (mapData.isEmpty()) {
            qCWarning(dcMap()) << "The map data file" << itemMap.value("data").toString() << "does not contains any valid data.";
            continue;
        }

        QString itemId = QString(QCryptographicHash::hash(itemMap.value("data").toString().toUtf8(), QCryptographicHash::Md5).toHex());
        QPoint position = QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt());

        QString itemTypeString = mapData.value("type").toString();
        if (itemTypeString == "plant") {
            gameItems.append(createPlantItem(itemId, mapData, position));
        } else if (itemTypeString == "weapon") {
            gameItems.append(createWeaponItem(itemId, mapData, position));
        } else if (itemTypeString == "firearm") {
            gameItems.append(createFirearmItem(itemId, mapData, position));
        } else if (itemTypeString == "tree") {
            gameItems.append(createTreeItem(itemId, mapData, position));
        } else if (itemTypeString == "character") {
            gameItems.append(createCharacterObject(itemId, mapData, position));
        } else if (itemTypeString == "enemy") {
            gameItems.append(createEnemyObject(itemId, mapData, position));
        } else if (itemTypeString == "chest") {
            gameItems.append(createChestItem(itemId, mapData, position));
        } else {
            qCWarning(dcMap()) << "Unhandled type" << itemTypeString;
        }
    }

    return gameItems;
}

PlantItem *DataLoader::createPlantItem(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    PlantItem *plantItem = new PlantItem();
    plantItem->setItemId(itemId);
    plantItem->setName(description.value("name").toString());
    plantItem->setImageName(description.value("imageName").toString());
    plantItem->setPosition(position);
    plantItem->setSize(itemSize);
    plantItem->setLayer(geometryMap.value("layer").toReal());

    plantItem->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    plantItem->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    plantItem->setPhysicsSize(physicsSize);
    plantItem->setPhysicsPosition(physicsPosition);
    plantItem->setCategoryFlag(categoryFlag);
    plantItem->setCollisionFlag(collisionFlag);

    plantItem->setPrice(description.value("price").toInt());
    plantItem->setHealing(description.value("healing").toInt());
    plantItem->setMana(description.value("mana").toInt());
    plantItem->setSpeed(description.value("speed").toInt());
    return plantItem;
}

TreeItem *DataLoader::createTreeItem(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    TreeItem *treeItem = new TreeItem();    
    treeItem->setItemId(itemId);
    treeItem->setName(description.value("name").toString());
    treeItem->setImageName(description.value("imageName").toString());
    treeItem->setPosition(position);
    treeItem->setLayer(geometryMap.value("layer").toReal());
    treeItem->setSize(itemSize);

    treeItem->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    treeItem->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    treeItem->setPhysicsSize(physicsSize);
    treeItem->setPhysicsPosition(physicsPosition);
    treeItem->setCategoryFlag(categoryFlag);
    treeItem->setCollisionFlag(collisionFlag);

    return treeItem;
}

WeaponItem *DataLoader::createWeaponItem(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    WeaponItem *weaponItem = new WeaponItem();
    weaponItem->setItemId(itemId);
    weaponItem->setName(description.value("name").toString());
    weaponItem->setImageName(description.value("imageName").toString());
    weaponItem->setPosition(position);
    weaponItem->setSize(itemSize);

    weaponItem->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    weaponItem->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    weaponItem->setPhysicsSize(physicsSize);
    weaponItem->setPhysicsPosition(physicsPosition);
    weaponItem->setCategoryFlag(categoryFlag);
    weaponItem->setCollisionFlag(collisionFlag);

    weaponItem->setLayer(geometryMap.value("layer").toReal());
    weaponItem->setDamage(description.value("damage").toInt());
    weaponItem->setPrice(description.value("price").toInt());

    return weaponItem;
}

FirearmItem *DataLoader::createFirearmItem(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    FirearmItem *firearmItem = new FirearmItem();
    firearmItem->setItemId(itemId);
    firearmItem->setName(description.value("name").toString());
    firearmItem->setImageName(description.value("imageName").toString());
    firearmItem->setPosition(position);
    firearmItem->setSize(itemSize);

    firearmItem->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    firearmItem->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    firearmItem->setPhysicsSize(physicsSize);
    firearmItem->setPhysicsPosition(physicsPosition);
    firearmItem->setCategoryFlag(categoryFlag);
    firearmItem->setCollisionFlag(collisionFlag);

    firearmItem->setLayer(geometryMap.value("layer").toReal());
    firearmItem->setDamage(description.value("damage").toInt());
    firearmItem->setPrice(description.value("price").toInt());

    return firearmItem;
}

Character *DataLoader::createCharacterObject(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    Character *character = new Character();
    character->setItemId(itemId);
    character->setName(description.value("name").toString());
    character->setPosition(position);
    character->setSize(itemSize);

    character->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    character->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    character->setPhysicsSize(physicsSize);
    character->setPhysicsPosition(physicsPosition);
    character->setCategoryFlag(categoryFlag);
    character->setCollisionFlag(collisionFlag);

    character->setGender(convertGenderString(description.value("gender", "male").toString()));
    character->setRole(convertRoleString(description.value("role", "player").toString()));
    character->setExperience(description.value("experience").toInt());
    character->setHealth(description.value("health").toInt());
    character->setHealthMax(description.value("healthMax").toInt());
    character->setMana(description.value("mana").toInt());
    character->setManaMax(description.value("manaMax").toInt());
    character->setWisdom(description.value("wisdom").toInt());
    character->setStrength(description.value("strength").toInt());
    character->setStrealth(description.value("stealth").toInt());

    // Items
    character->inventory()->addGameItemList(loadGameItems(description.value("items").toList()));

    return character;
}

Enemy *DataLoader::createEnemyObject(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    Enemy *enemy = new Enemy();
    enemy->setItemId(itemId);
    enemy->setName(description.value("name").toString());
    enemy->setImageName(description.value("imageName").toString());
    enemy->setPosition(position);
    enemy->setSize(itemSize);

    enemy->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    enemy->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    enemy->setPhysicsSize(physicsSize);
    enemy->setPhysicsPosition(physicsPosition);
    enemy->setCategoryFlag(categoryFlag);
    enemy->setCollisionFlag(collisionFlag);

    enemy->setGender(convertGenderString(description.value("gender", "male").toString()));
    enemy->setRole(convertRoleString(description.value("role", "player").toString()));
    enemy->setExperience(description.value("experience").toInt());
    enemy->setHealth(description.value("health").toInt());
    enemy->setHealthMax(description.value("healthMax").toInt());
    enemy->setMana(description.value("mana").toInt());
    enemy->setManaMax(description.value("manaMax").toInt());
    enemy->setWisdom(description.value("wisdom").toInt());
    enemy->setStrength(description.value("strength").toInt());
    enemy->setStrealth(description.value("stealth").toInt());
    enemy->inventory()->addGameItemList(loadGameItems(description.value("items").toList()));

    enemy->setTouchDamage(description.value("touchDamage").toInt());
    enemy->setShootDamage(description.value("shootDamage").toInt());
    enemy->setHitDamage(description.value("hitDamage").toInt());

    // Items
    enemy->inventory()->addGameItemList(loadGameItems(description.value("items").toList()));

    return enemy;
}

ChestItem *DataLoader::createChestItem(const QString &itemId, const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    QSize physicsSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt());
    QPointF physicsPosition(physicsGeometryMap.value("x", 0).toInt(), physicsGeometryMap.value("y", 0).toInt());
    GameObject::PhysicsFlags categoryFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt());
    GameObject::PhysicsFlags collisionFlag = static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt());

    ChestItem *chestItem = new ChestItem();
    chestItem->setItemId(itemId);
    chestItem->setName(description.value("name").toString());
    chestItem->setImageName(description.value("imageName").toString());
    chestItem->setPosition(position);
    chestItem->setSize(itemSize);

    chestItem->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    chestItem->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    chestItem->setPhysicsSize(physicsSize);
    chestItem->setPhysicsPosition(physicsPosition);
    chestItem->setCategoryFlag(categoryFlag);
    chestItem->setCollisionFlag(collisionFlag);

    // Items
    chestItem->items()->addGameItemList(loadGameItems(description.value("items").toList()));

    // Lock combination
    QStringList lockCombination = description.value("lockCombination").toStringList();
    qCDebug(dcItem()) << chestItem << lockCombination;
    if (lockCombination.isEmpty())
        chestItem->setLocked(false);
    else {
        chestItem->setLocked(true);
        chestItem->setLockCombination(lockCombination);
    }

    return chestItem;
}

QList<QPoint> DataLoader::loadFieldMap(const QVariantList &fieldMap)
{
    QList<QPoint> pointList;
    foreach(const QVariant &pointVariant, fieldMap) {
        QVariantMap pointMap = pointVariant.toMap();
        pointList.append(QPoint(pointMap.value("x").toInt(), pointMap.value("y").toInt()));
    }
    return pointList;
}

QVariantMap DataLoader::loadJsonData(const QString &mapDataFileName)
{
    QFile mapDataFile(mapDataFileName);
    if (!mapDataFile.open(QFile::ReadOnly)) {
        qCWarning(dcMap()) << "Could not open map data file" << mapDataFileName << "for reading.";
        return QVariantMap();
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(mapDataFile.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(dcMap()) << "Cannot parse map data file:" << error.errorString();
        return QVariantMap();
    }

    return jsonDoc.toVariant().toMap();
}

GameObject::Shape DataLoader::convertShapeString(const QString &shapeString)
{
    GameObject::Shape shape = GameObject::ShapeNone;
    if (shapeString.toLower() == "circle") {
        shape = GameObject::ShapeCircle;
    } else if (shapeString.toLower() == "point") {
        shape = GameObject::ShapePoint;
    } else if (shapeString.toLower() == "rectangle") {
        shape = GameObject::ShapeRectangle;
    } else if (shapeString.toLower() == "polygon") {
        shape = GameObject::ShapePolygon;
    }

    return shape;
}

GameObject::BodyType DataLoader::convertBodyTypeString(const QString &bodyTypeString)
{
    GameObject::BodyType bodyType = GameObject::BodyTypeStatic;
    if (bodyTypeString.toLower() == "static") {
        bodyType = GameObject::BodyTypeStatic;
    } else if (bodyTypeString.toLower() == "dynamic") {
        bodyType = GameObject::BodyTypeDynamic;
    } else if (bodyTypeString.toLower() == "kinematic") {
        bodyType = GameObject::BodyTypeKinematic;
    }

    return bodyType;
}

Character::Gender DataLoader::convertGenderString(const QString &genderString)
{
    Character::Gender gender = Character::Male;
    if (genderString.toLower() == "male") {
        gender = Character::Male;
    } else if (genderString.toLower() == "female") {
        gender = Character::Female;
    }
    return gender;
}

Character::Role DataLoader::convertRoleString(const QString &roleString)
{
    Character::Role role = Character::None;
    if (roleString.toLower() == "player") {
        role = Character::Player;
    } else if (roleString.toLower() == "statist") {
        role = Character::Statist;
    } else if (roleString.toLower() == "friend") {
        role = Character::Friend;
    } else if (roleString.toLower() == "enemy") {
        role = Character::Enemy;
    } else if (roleString.toLower() == "magician") {
        role = Character::Magician;
    } else if (roleString.toLower() == "warrior") {
        role = Character::Warrior;
    } else if (roleString.toLower() == "dealer") {
        role = Character::Dealer;
    } else {
        qCWarning(dcMap()) << "Invalid role" << roleString;
    }
    return role;
}
