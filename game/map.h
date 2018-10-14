#ifndef MAP_H
#define MAP_H

#include <QList>
#include <QSize>
#include <QObject>
#include <QAbstractItemModel>

#include "fields.h"
#include "items/treeitem.h"
#include "items/gameitems.h"
#include "items/plantitem.h"
#include "items/weaponitem.h"

class Map : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)
    Q_PROPERTY(QPointF playerStartPosition READ playerStartPosition NOTIFY playerStartPositionChanged)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)
    Q_PROPERTY(QString fileName READ fileName NOTIFY fileNameChanged)

public:
    enum Layer {
        Layer0Lowest = 0,
        Layer1Lower = 1,
        Layer2Normal = 2,
        Layer3Higher = 3,
        Layer4Highest = 4,

    };
    Q_ENUM(Layer)

    explicit Map(QObject *parent = nullptr);
    ~Map();

    QSize size() const;
    void setSize(const QSize &size);

    QPointF playerStartPosition() const;
    void setPlayerStartPosition(const QPointF &playerStartPosition);

    QString name() const;
    void setName(const QString &name);

    QString fileName() const;

    QList<GameItem *> items();

    void loadMap(const QString &fileName);

    Q_INVOKABLE Field *getField(int x, int y) const;
    Q_INVOKABLE Field *getField(const QPointF position) const;

private:
    QSize m_size;
    QPointF m_playerStartPosition;
    QString m_name;
    QString m_fileName;

    QList<Fields *> m_mapData;
    QList<GameItem *> m_items;
    QList<PlantItem *> m_plants;
    QList<WeaponItem *> m_weapons;
    QList<TreeItem *> m_trees;

    QVariantMap loadMapData(const QString &mapDataFileName);

    void placeItemOnMap(GameItem *item);

    PlantItem *createPlantItem(const QVariantMap &plantItemMap);
    TreeItem *createTreeItem(const QVariantMap &treeItemMap);
    WeaponItem *createWeaponItem(const QVariantMap &weaponItemMap);
    QList<QPoint> loadFieldMap(const QVariantList &fieldMap);

signals:
    void sizeChanged(const QSize &size);
    void playerStartPositionChanged(const QPointF &playerStartPosition);
    void nameChanged(const QString &name);
    void fileNameChanged(const QString &fileName);

public slots:
    void clear();


};

#endif // MAP_H
