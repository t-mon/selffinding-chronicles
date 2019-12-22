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
#include "items/literatureitem.h"
#include "items/teleporteritem.h"
#include "items/boxitem.h"

class DataLoader
{
public:
    explicit DataLoader() = default;

    // Create
    static StaticItem *createStaticItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static PlantItem *createPlantItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static TreeItem *createTreeItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static WeaponItem *createWeaponItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static FirearmItem *createFirearmItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static LiteratureItem *createLiteratureItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static TeleporterItem *createTeleportItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static BoxItem *createBoxItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);

    static Character *createCharacterObject(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static Enemy *createEnemyObject(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static ChestItem *createChestItem(const QString &resourcePath, const QVariantMap &description, const QPointF &position = QPointF(), QObject *parent = nullptr);
    static Path *createPathObject(const QVariantMap &description, QObject *parent);

    // Load
    static QList<QPointF> loadFieldMap(const QVariantList &fieldMap);
    static QVariantMap loadJsonData(const QString &dataFileName);

    static QList<GameItem *> loadGameItems(const QVariantList &itemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadChestItems(const QVariantList &chestItemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadCharacterItems(const QVariantList &characterItemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadEnemyItems(const QVariantList &enemyItemsList, QObject *parent = nullptr);
    static QList<GameItem *> loadInventoryItems(const QVariantList &itemsList, QObject *parent = nullptr);    

    static GameItem *loadGameItem(const QString &resourcePath, const QPointF &position, const QVariantMap &itemMap, QObject *parent = nullptr);
    static GameItem *loadGameItemFromResourcePath(const QString &resourcePath, QObject *parent = nullptr);

    // Load object
    static GameObject *loadGameObject(const QString &resourcePath, const QPointF &position, const QVariantMap &objectMap, QObject *parent = nullptr);
    static GameObject *loadGameObjectFromResourcePath(const QString &resourcePath, QObject *parent = nullptr);

    static QList<GameObject *> loadGameObjects(const QVariantList &objectsList, QObject *parent = nullptr);


    static void fillGameItemData(GameItem *item, const QVariantMap &description);
    static void fillGameObjectData(GameObject *object, const QVariantMap &description);
    static void fillCharacterItemData(Character *character, const QVariantMap &characterMap);
    static void fillChestItemData(ChestItem *chestItem, const QVariantMap &chestMap);

    // Save
    static QVariantMap gameItemToVariantMap(GameItem *item);
    static QVariantList gameItemsToVariantList(GameItems *items);
    static QVariantMap enemieToVariantMap(Enemy *enemy);

    static QVariantList charactersToVariantList(GameItems *characters);
    static QVariantMap characterToVariantMap(Character *character);

    static QVariantList pathsToVariantList(const QList<Path *> paths);
    static QVariantMap pathToVariantMap(Path *path);

    static QVariantList pathSegmentsToVariantList(const QList<PathSegment> &pathSegments);
    static QVariantMap pathSegmentToVariantMap(const PathSegment &pathSegment);

    // Convert
    static GameObject::Layer convertLayerValue(int layerValue);
    static GameObject::Shape convertShapeString(const QString &shapeString);
    static GameObject::BodyType convertBodyTypeString(const QString &bodyTypeString);
    static FirearmItem::FirearmType convertFirearmTypeString(const QString &firearmTypeString);
    static LiteratureItem::LiteratureType convertLiteratureTypeString(const QString &literatureTypeString);
    static QString getItemIdFromResourcePath(const QString &resourcePath);

    static PathSegment::Type convertPathSegmentTypeString(const QString &pathSegmentTypeString);
    static QString pathSegmentTypeToString(PathSegment::Type pathSegmentType);

    static Character::Gender convertGenderString(const QString &genderString);
    static QString genderToString(Character::Gender gender);

    static Character::Role convertRoleString(const QString &roleString);
    static QString roleToString(Character::Role role);

};

#endif // DATALOADER_H
