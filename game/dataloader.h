#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QVariantMap>

#include "path.h"
#include "items/staticitem.h"
#include "items/treeitem.h"
#include "items/gameitems.h"
#include "items/plantitem.h"
#include "items/weaponitem.h"
#include "items/firearmitem.h"
#include "items/character.h"
#include "items/enemy.h"
#include "items/chestitem.h"

class DataLoader
{
public:
    explicit DataLoader() = default;


    // Create
    static StaticItem *createStaticItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static PlantItem *createPlantItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static TreeItem *createTreeItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static WeaponItem *createWeaponItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static FirearmItem *createFirearmItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);

    static Character *createCharacterObject(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static Enemy *createEnemyObject(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static ChestItem *createChestItem(const QString &resourcePath, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static Path *createPathObject(const QVariantMap &description, QObject *parent);

    // Load
    static QList<QPoint> loadFieldMap(const QVariantList &fieldMap);
    static QVariantMap loadJsonData(const QString &dataFileName);

    static QList<GameItem *> loadGameItems(const QVariantList &itemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadChestItems(const QVariantList &chestItemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadCharacterItems(const QVariantList &characterItemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadEnemyItems(const QVariantList &enemyItemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadInventoryItems(const QVariantList &itemsList, QObject *parent = nullptr);    

    static GameItem *loadGameItem(const QString &resourcePath, const QPoint &position, const QVariantMap &itemMap, QObject *parent = nullptr);
    static GameItem *loadGameItemFromResourcePath(const QString &resourcePath, QObject *parent = nullptr);

    static void fillGameItemData(GameItem *item, const QVariantMap &description);
    static void fillCharacterItemData(Character *character, const QVariantMap &characterMap);
    static void fillChestItemData(ChestItem *chestItem, const QVariantMap &chestMap);

    // Convert
    static GameObject::Shape convertShapeString(const QString &shapeString);
    static GameObject::BodyType convertBodyTypeString(const QString &bodyTypeString);
    static Character::Gender convertGenderString(const QString &genderString);
    static Character::Role convertRoleString(const QString &roleString);
    static FirearmItem::FirearmType convertFirearmTypeString(const QString &firearmTypeString);
    static PathSegment::Type convertPathSegmentTypeString(const QString &pathSegmentTypeString);
    static QString getItemIdFromResourcePath(const QString &resourcePath);
};

#endif // DATALOADER_H
