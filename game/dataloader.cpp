#include "dataloader.h"
#include "debugcategories.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonParseError>

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

        QString itemTypeString = mapData.value("type").toString();
        if (itemTypeString == "plant") {
            gameItems.append(createPlantItem(mapData, QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt())));
        } else if (itemTypeString == "weapon") {
            gameItems.append(createWeaponItem(mapData, QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt())));
        } else if (itemTypeString == "tree") {
            gameItems.append(createTreeItem(mapData, QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt())));
        } else if (itemTypeString == "character") {
            gameItems.append(createCharacterObject(mapData, QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt())));
        } else if (itemTypeString == "enemy") {
            gameItems.append(createEnemyObject(mapData, QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt())));
        } else if (itemTypeString == "chest") {
            gameItems.append(createChestItem(mapData, QPoint(itemMap.value("x", -1).toInt(), itemMap.value("y", -1).toInt())));
        } else {
            qCWarning(dcMap()) << "Unhandled type" << itemTypeString;
        }
    }

    return gameItems;
}

PlantItem *DataLoader::createPlantItem(const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    PlantItem *plantItem = new PlantItem();
    plantItem->setPosition(position);
    plantItem->setSize(itemSize);
    plantItem->setShape(PlantItem::ShapeCircle);
    plantItem->setLayer(geometryMap.value("layer").toReal());
    plantItem->setName(description.value("name").toString());
    plantItem->setImageName(description.value("imageName").toString());
    plantItem->setPrice(description.value("price").toInt());
    plantItem->setHealing(description.value("healing").toInt());
    plantItem->setMana(description.value("mana").toInt());
    plantItem->setSpeed(description.value("speed").toInt());
    return plantItem;
}

TreeItem *DataLoader::createTreeItem(const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    TreeItem *treeItem = new TreeItem();
    treeItem->setPosition(position);
    treeItem->setLayer(geometryMap.value("layer").toReal());
    treeItem->setSize(itemSize);
    treeItem->setName(description.value("name").toString());
    treeItem->setImageName(description.value("imageName").toString());

    return treeItem;
}

WeaponItem *DataLoader::createWeaponItem(const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    WeaponItem *weaponItem = new WeaponItem();
    weaponItem->setPosition(position);
    weaponItem->setSize(itemSize);
    weaponItem->setShape(PlantItem::ShapeCircle);
    weaponItem->setLayer(geometryMap.value("layer").toReal());
    weaponItem->setName(description.value("name").toString());
    weaponItem->setImageName(description.value("imageName").toString());
    weaponItem->setDamage(description.value("damage").toInt());
    weaponItem->setPrice(description.value("price").toInt());

    return weaponItem;
}

Character *DataLoader::createCharacterObject(const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    Character *character = new Character();
    character->setName(description.value("name").toString());
    character->setPosition(position);
    character->setSize(itemSize);
    character->setShape(PlantItem::ShapeCircle);

    if (description.value("gender", "male").toString().toLower() == "male") {
        character->setGender(Character::Male);
    } else if (description.value("gender", "male").toString().toLower() == "female") {
        character->setGender(Character::Female);
    }

    QString roleName = description.value("role", "player").toString().toLower();
    if (roleName == "player") {
        character->setRole(Character::Player);
    } else if (roleName == "statist") {
        character->setRole(Character::Statist);
    } else if (roleName == "friend") {
        character->setRole(Character::Friend);
    } else if (roleName == "enemy") {
        character->setRole(Character::Enemy);
    } else if (roleName == "magician") {
        character->setRole(Character::Magician);
    } else if (roleName == "warrior") {
        character->setRole(Character::Warrior);
    } else if (roleName == "dealer") {
        character->setRole(Character::Dealer);
    } else {
        qCWarning(dcMap()) << "Invalid role" << roleName;
    }

    character->setExperience(description.value("experience").toInt());
    character->setHealth(description.value("health").toInt());
    character->setHealthMax(description.value("healthMax").toInt());
    character->setMana(description.value("mana").toInt());
    character->setManaMax(description.value("manaMax").toInt());
    character->setWisdom(description.value("wisdom").toInt());
    character->setStrength(description.value("strength").toInt());
    character->setStrealth(description.value("stealth").toInt());
    character->inventory()->addGameItemList(loadGameItems(description.value("items").toList()));

    return character;
}

Enemy *DataLoader::createEnemyObject(const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    Enemy *enemy = new Enemy();
    enemy->setName(description.value("name").toString());
    enemy->setPosition(position);
    enemy->setSize(itemSize);
    enemy->setShape(PlantItem::ShapeCircle);

    if (description.value("gender", "male").toString().toLower() == "male") {
        enemy->setGender(Character::Male);
    } else if (description.value("gender", "male").toString().toLower() == "female") {
        enemy->setGender(Character::Female);
    }

    QString roleName = description.value("role", "player").toString().toLower();
    if (roleName == "player") {
        enemy->setRole(Character::Player);
    } else if (roleName == "statist") {
        enemy->setRole(Character::Statist);
    } else if (roleName == "friend") {
        enemy->setRole(Character::Friend);
    } else if (roleName == "enemy") {
        enemy->setRole(Character::Enemy);
    } else if (roleName == "magician") {
        enemy->setRole(Character::Magician);
    } else if (roleName == "warrior") {
        enemy->setRole(Character::Warrior);
    } else if (roleName == "dealer") {
        enemy->setRole(Character::Dealer);
    } else {
        qCWarning(dcMap()) << "Invalid role" << roleName;
    }

    enemy->setImageName(description.value("imageName").toString());
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

    return enemy;
}

ChestItem *DataLoader::createChestItem(const QVariantMap &description, const QPoint &position)
{
    QVariantMap geometryMap = description.value("geometry").toMap();
    QSize itemSize(geometryMap.value("width", 1).toInt(), geometryMap.value("height", 1).toInt());

    ChestItem *chestItem = new ChestItem();
    chestItem->setName(description.value("name").toString());
    chestItem->setPosition(position);
    chestItem->setSize(itemSize);
    chestItem->setShape(PlantItem::ShapeCircle);
    chestItem->setName(description.value("name").toString());
    chestItem->setImageName(description.value("imageName").toString());

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
