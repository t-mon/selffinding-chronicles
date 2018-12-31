#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>

#include "items/treeitem.h"
#include "items/gameitems.h"
#include "items/plantitem.h"
#include "items/weaponitem.h"
#include "items/character.h"
#include "items/enemy.h"
#include "items/chestitem.h"

class DataLoader
{
public:
    explicit DataLoader() = default;

    static QList<GameItem *> loadGameItems(const QVariantList &itemsList);

    static PlantItem *createPlantItem(const QVariantMap &description, const QPoint &position = QPoint());
    static TreeItem *createTreeItem(const QVariantMap &description, const QPoint &position = QPoint());
    static WeaponItem *createWeaponItem(const QVariantMap &description, const QPoint &position = QPoint());
    static Character *createCharacterObject(const QVariantMap &description, const QPoint &position = QPoint());
    static Enemy *createEnemyObject(const QVariantMap &description, const QPoint &position = QPoint());
    static ChestItem *createChestItem(const QVariantMap &description, const QPoint &position = QPoint());

    static QList<QPoint> loadFieldMap(const QVariantList &fieldMap);
    static QVariantMap loadJsonData(const QString &mapDataFileName);

    static GameObject::Shape convertShapeString(const QString &shapeString);
    static GameObject::BodyType convertBodyTypeString(const QString &bodyTypeString);
    static Character::Gender convertGenderString(const QString &genderString);
    static Character::Role convertRoleString(const QString &roleString);

};

#endif // DATALOADER_H
