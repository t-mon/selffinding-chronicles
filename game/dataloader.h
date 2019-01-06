#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

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

    static QList<GameItem *> loadGameItems(const QVariantList &itemsList, QObject *parent = nullptr);

    // Create
    static PlantItem *createPlantItem(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static TreeItem *createTreeItem(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static WeaponItem *createWeaponItem(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static FirearmItem *createFirearmItem(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static Character *createCharacterObject(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static Enemy *createEnemyObject(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);
    static ChestItem *createChestItem(const QString &itemId, const QVariantMap &description, const QPoint &position = QPoint(), QObject *parent = nullptr);

    // Load
    static QList<QPoint> loadFieldMap(const QVariantList &fieldMap);
    static QVariantMap loadJsonData(const QString &mapDataFileName);

    // Convert
    static GameObject::Shape convertShapeString(const QString &shapeString);
    static GameObject::BodyType convertBodyTypeString(const QString &bodyTypeString);
    static Character::Gender convertGenderString(const QString &genderString);
    static Character::Role convertRoleString(const QString &roleString);

};

#endif // DATALOADER_H
