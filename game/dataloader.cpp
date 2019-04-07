#include "gameobject.h"
#include "dataloader.h"
#include "debugcategories.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QCryptographicHash>

StaticItem *DataLoader::createStaticItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    StaticItem *item = new StaticItem(parent);
    item->setResourcePath(resourcePath);
    item->setItemId(getItemIdFromResourcePath(resourcePath));
    item->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(item), description);

    return item;
}

PlantItem *DataLoader::createPlantItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    PlantItem *plantItem = new PlantItem(parent);
    plantItem->setResourcePath(resourcePath);
    plantItem->setItemId(getItemIdFromResourcePath(resourcePath));
    plantItem->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(plantItem), description);

    QVariantMap plantMap = description.value("plant").toMap();
    plantItem->setPrice(plantMap.value("price").toInt());
    plantItem->setHealing(plantMap.value("healing").toInt());
    plantItem->setMana(plantMap.value("mana").toInt());
    plantItem->setSpeed(plantMap.value("speed").toInt());

    return plantItem;
}

TreeItem *DataLoader::createTreeItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    TreeItem *treeItem = new TreeItem(parent);
    treeItem->setResourcePath(resourcePath);
    treeItem->setItemId(getItemIdFromResourcePath(resourcePath));
    treeItem->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(treeItem), description);

    return treeItem;
}

WeaponItem *DataLoader::createWeaponItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    WeaponItem *weaponItem = new WeaponItem(parent);
    weaponItem->setResourcePath(resourcePath);
    weaponItem->setItemId(getItemIdFromResourcePath(resourcePath));
    weaponItem->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(weaponItem), description);

    QVariantMap weaponMap = description.value("weapon").toMap();
    weaponItem->setDamage(weaponMap.value("damage").toInt());
    weaponItem->setPrice(weaponMap.value("price").toInt());

    return weaponItem;
}

FirearmItem *DataLoader::createFirearmItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    FirearmItem *firearmItem = new FirearmItem(parent);
    firearmItem->setResourcePath(resourcePath);
    firearmItem->setItemId(getItemIdFromResourcePath(resourcePath));
    firearmItem->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(firearmItem), description);

    QVariantMap firearmMap = description.value("firearm").toMap();
    firearmItem->setFirearmType(convertFirearmTypeString(firearmMap.value("type").toString()));
    firearmItem->setDamage(firearmMap.value("damage").toInt());
    firearmItem->setRange(firearmMap.value("range").toInt());
    firearmItem->setPrice(firearmMap.value("price").toInt());

    return firearmItem;
}

Character *DataLoader::createCharacterObject(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    Character *character = new Character(parent);
    character->setResourcePath(resourcePath);
    character->setItemId(getItemIdFromResourcePath(resourcePath));
    character->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(character), description);
    fillCharacterItemData(character, description.value("character").toMap());

    return character;
}

Enemy *DataLoader::createEnemyObject(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    Enemy *enemy = new Enemy(parent);
    enemy->setResourcePath(resourcePath);
    enemy->setItemId(getItemIdFromResourcePath(resourcePath));
    enemy->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(enemy), description);
    fillCharacterItemData(qobject_cast<Character *>(enemy), description.value("character").toMap());

    QVariantMap enemyMap = description.value("enemy").toMap();
    enemy->setTouchDamage(enemyMap.value("touchDamage").toInt());
    enemy->setShootDamage(enemyMap.value("shootDamage").toInt());
    enemy->setHitDamage(enemyMap.value("hitDamage").toInt());

    return enemy;
}

ChestItem *DataLoader::createChestItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position, QObject *parent)
{
    ChestItem *chestItem = new ChestItem(parent);
    chestItem->setResourcePath(resourcePath);
    chestItem->setItemId(getItemIdFromResourcePath(resourcePath));
    chestItem->setPosition(position);
    fillGameItemData(qobject_cast<GameItem *>(chestItem), description);

    // Chest properties
    QVariantMap chestMap = description.value("chest").toMap();
    fillChestItemData(chestItem, chestMap);

    return chestItem;
}

Path *DataLoader::createPathObject(const QVariantMap &description, QObject *parent)
{
    Path *path = new Path(parent);
    path->setId(description.value("id").toString());

    // Load segments
    QList<PathSegment> pathSegments;
    foreach (const QVariant &segmentVariant, description.value("segments").toList()) {
        PathSegment segment; QVariantMap segmentDescription = segmentVariant.toMap();
        segment.setType(convertPathSegmentTypeString(segmentDescription.value("type", "segment").toString()));
        segment.setOffset(QPointF(segmentDescription.value("offsetX", 0).toDouble(), segmentDescription.value("offsetY", 0).toDouble()));
        segment.setSpeed(segmentDescription.value("speed", 0).toDouble());
        segment.setAngle(segmentDescription.value("angle", 0).toDouble());
        segment.setDuration(segmentDescription.value("duration", 0).toInt());
        pathSegments.append(segment);
    }

    path->setPathSegments(pathSegments);
    return path;
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

QVariantMap DataLoader::loadJsonData(const QString &dataFileName)
{
    QFile mapDataFile(dataFileName);
    if (!mapDataFile.open(QFile::ReadOnly)) {
        qCWarning(dcMap()) << "Could not open map data file" << dataFileName << "for reading.";
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

QList<GameItem *> DataLoader::loadGameItems(const QVariantList &itemsList, QObject *parent)
{
    QList<GameItem *> gameItems;

    foreach (const QVariant &itemVariant, itemsList) {
        QVariantMap itemMap = itemVariant.toMap();
        GameItem *gameItem = loadGameItemFromResourcePath(itemMap.value("data").toString(), parent);
        gameItem->setPosition(QPointF(itemMap.value("x", -1).toDouble(), itemMap.value("y", -1).toDouble()));
        gameItems.append(gameItem);
    }

    return gameItems;
}

QList<GameItem *> DataLoader::loadChestItems(const QVariantList &chestItemsList, QObject *parent)
{
    QList<GameItem *> gameItems;

    foreach (const QVariant &itemVariant, chestItemsList) {
        QVariantMap itemMap = itemVariant.toMap();
        GameItem *gameItem = loadGameItemFromResourcePath(itemMap.value("data").toString(), parent);
        gameItem->setPosition(QPointF(itemMap.value("x", -1).toDouble(), itemMap.value("y", -1).toDouble()));
        if (itemMap.contains("chest")) {
            QVariantMap chestMap = itemMap.value("chest").toMap();
            fillChestItemData(qobject_cast<ChestItem *>(gameItem), chestMap);
        }

        gameItems.append(gameItem);
    }

    return gameItems;
}

QList<GameItem *> DataLoader::loadCharacterItems(const QVariantList &characterItemsList, QObject *parent)
{
    QList<GameItem *> gameItems;

    foreach (const QVariant &itemVariant, characterItemsList) {
        QVariantMap itemMap = itemVariant.toMap();
        GameItem *gameItem = loadGameItemFromResourcePath(itemMap.value("data").toString(), parent);
        gameItem->setPosition(QPointF(itemMap.value("x", -1).toDouble(), itemMap.value("y", -1).toDouble()));
        if (itemMap.contains("character")) {
            QVariantMap characterMap = itemMap.value("character").toMap();
            fillCharacterItemData(qobject_cast<Character *>(gameItem), characterMap);
        }

        gameItems.append(gameItem);
    }

    return gameItems;
}

QList<GameItem *> DataLoader::loadEnemyItems(const QVariantList &enemyItemsList, QObject *parent)
{
    QList<GameItem *> gameItems;

    foreach (const QVariant &itemVariant, enemyItemsList) {
        QVariantMap itemMap = itemVariant.toMap();
        GameItem *gameItem = loadGameItemFromResourcePath(itemMap.value("data").toString(), parent);
        gameItem->setPosition(QPointF(itemMap.value("x", -1).toDouble(), itemMap.value("y", -1).toDouble()));
        if (itemMap.contains("character")) {
            QVariantMap characterMap = itemMap.value("character").toMap();
            fillCharacterItemData(qobject_cast<Character *>(gameItem), characterMap);
        }

        gameItems.append(gameItem);
    }

    return gameItems;
}

QList<GameItem *> DataLoader::loadInventoryItems(const QVariantList &itemsList, QObject *parent)
{
    QList<GameItem *> gameItems;

    foreach (const QVariant &itemVariant, itemsList) {
        QVariantMap itemMap = itemVariant.toMap();
        int count = itemMap.value("count").toInt();
        for (int i = 0; i < count; i++) {
            gameItems.append(loadGameItemFromResourcePath(itemMap.value("data").toString(), parent));
        }
    }

    return gameItems;
}

GameItem *DataLoader::loadGameItem(const QString &resourcePath, const QPoint &position, const QVariantMap &itemMap, QObject *parent)
{
    QString itemTypeString = itemMap.value("type").toString().toLower();

    if (itemTypeString == "static") {
        return createStaticItem(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "plant") {
        return createPlantItem(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "weapon") {
        return createWeaponItem(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "firearm") {
        return createFirearmItem(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "tree") {
        return createTreeItem(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "character") {
        return createCharacterObject(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "enemy") {
        return createEnemyObject(resourcePath, itemMap, position, parent);
    } else if (itemTypeString == "chest") {
        return createChestItem(resourcePath, itemMap, position, parent);
    } else {
        qCWarning(dcMap()) << "Unhandled type" << itemTypeString;
    }

    return nullptr;
}

GameItem *DataLoader::loadGameItemFromResourcePath(const QString &resourcePath, QObject *parent)
{
    QVariantMap itemMap = loadJsonData(resourcePath);
    return loadGameItem(resourcePath, QPoint(-1, -1), itemMap, parent);
}

void DataLoader::fillGameItemData(GameItem *item, const QVariantMap &description)
{
    // Game item properties
    QVariantMap geometryMap = description.value("geometry").toMap();
    QVariantMap physicsGeometryMap = description.value("physicsGeometry").toMap();
    item->setName(description.value("name").toString());
    item->setImageName(description.value("imageName").toString());
    item->setSize(QSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt()));
    item->setLayer(geometryMap.value("layer").toReal());
    item->setShape(convertShapeString(physicsGeometryMap.value("shape", "none").toString()));
    item->setBodyType(convertBodyTypeString(physicsGeometryMap.value("body", "static").toString()));
    item->setPhysicsSize(QSize(physicsGeometryMap.value("width", 0).toInt(), physicsGeometryMap.value("height", 0).toInt()));
    item->setPhysicsPosition(QPointF(physicsGeometryMap.value("x", 0).toDouble(), physicsGeometryMap.value("y", 0).toDouble()));
    item->setCategoryFlag(static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("category", 0).toInt()));
    item->setCollisionFlag(static_cast<GameObject::PhysicsFlags>(physicsGeometryMap.value("collision", 0).toInt()));
}

void DataLoader::fillCharacterItemData(Character *character, const QVariantMap &characterMap)
{
    // Character properties
    character->setGender(convertGenderString(characterMap.value("gender", "male").toString()));
    character->setRole(convertRoleString(characterMap.value("role", "player").toString()));
    character->setExperience(characterMap.value("experience").toInt());
    character->setHealth(characterMap.value("health").toInt());
    character->setHealthMax(characterMap.value("healthMax").toInt());
    character->setMana(characterMap.value("mana").toInt());
    character->setManaMax(characterMap.value("manaMax").toInt());
    character->setWisdom(characterMap.value("wisdom").toInt());
    character->setStrength(characterMap.value("strength").toInt());
    character->setStrealth(characterMap.value("stealth").toInt());

    // Inventory
    character->inventory()->addGameItemList(loadInventoryItems(characterMap.value("inventory").toList(), character));

    // Arm the character
    QString weaponResourcePath = characterMap.value("weapon").toString();
    if (!weaponResourcePath.isEmpty()) {
        GameItem *weapon = character->inventory()->getFirstGameItem(getItemIdFromResourcePath(weaponResourcePath));
        if (!weapon) {
            qCWarning(dcDataManager()) << "Character" << character << "configured a weapon which is not in the inventory";
        } else {
            character->setWeapon(qobject_cast<WeaponItem *>(weapon));
        }
    }

    QString firearmResourcePath = characterMap.value("firearm").toString();
    if (!firearmResourcePath.isEmpty()) {
        GameItem *firearm = character->inventory()->getFirstGameItem(getItemIdFromResourcePath(firearmResourcePath));
        if (!firearm) {
            qCWarning(dcDataManager()) << "Character" << character << "configured a firearm which is not in the inventory";
        } else {
            character->setFirearm(qobject_cast<FirearmItem *>(firearm));
        }
    }

    // Load the paths
    QList<Path *> paths;
    foreach (const QVariant &pathVariant, characterMap.value("paths").toList()) {
        paths.append(createPathObject(pathVariant.toMap(), character));
        qCDebug(dcMap()) << paths.last();
    }
    character->setPaths(paths);
}

void DataLoader::fillChestItemData(ChestItem *chestItem, const QVariantMap &chestMap)
{
    chestItem->items()->addGameItemList(loadInventoryItems(chestMap.value("inventory").toList(), chestItem));
    chestItem->setLockCombination(chestMap.value("lockCombination").toStringList());
    chestItem->setLocked(chestMap.value("locked", false).toBool());
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

FirearmItem::FirearmType DataLoader::convertFirearmTypeString(const QString &firearmTypeString)
{
    FirearmItem::FirearmType type = FirearmItem::FirearmTypeBow;
    if (firearmTypeString.toLower() == "bow") {
        type = FirearmItem::FirearmTypeBow;
    } else if (firearmTypeString.toLower() == "crossbow") {
        type = FirearmItem::FirearmTypeCrossbow;
    } else {
        qCWarning(dcMap()) << "Invalid firearm type" << firearmTypeString;
    }
    return type;
}

PathSegment::Type DataLoader::convertPathSegmentTypeString(const QString &pathSegmentTypeString)
{
    PathSegment::Type type = PathSegment::TypeSegment;
    if (pathSegmentTypeString.toLower() == "segment") {
        type = PathSegment::TypeSegment;
    } else if (pathSegmentTypeString.toLower() == "rotate") {
        type = PathSegment::TypeRotate;
    } else if (pathSegmentTypeString.toLower() == "pause") {
        type = PathSegment::TypePause;
    } else {
        qCWarning(dcMap()) << "Invalid path segment type" << pathSegmentTypeString;
    }
    return type;
}

QString DataLoader::getItemIdFromResourcePath(const QString &resourcePath)
{
    return QString(QCryptographicHash::hash(resourcePath.toUtf8(), QCryptographicHash::Md5).toHex());
}
