#ifndef MAP_H
#define MAP_H

#include <QList>
#include <QSize>
#include <QObject>
#include <QAbstractItemModel>

#include "fields.h"
#include "dataloader.h"
#include "gameobjects.h"
#include "items/treeitem.h"
#include "items/gameitems.h"
#include "items/plantitem.h"
#include "items/weaponitem.h"
#include "items/weatherareaproxy.h"

class Map : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSize size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(QPointF playerStartPosition READ playerStartPosition WRITE setPlayerStartPosition NOTIFY playerStartPositionChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString resourceFileName READ resourceFileName NOTIFY resourceFileNameChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)

public:
    explicit Map(QObject *parent = nullptr);
    Map(GameObjects *objects, GameItems *items, GameItems *enemies, GameItems *characters, QObject *parent = nullptr);
    ~Map();

    QSize size() const;
    void setSize(const QSize &size);

    QPointF playerStartPosition() const;
    void setPlayerStartPosition(const QPointF &playerStartPosition);

    QString name() const;
    void setName(const QString &name);

    QString resourcePath() const;
    void setResourcePath(const QString &resourcePath);

    QString resourceFileName() const;

    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    Character *player() const;
    void setPlayer(Character *player);

    GameObjects *objects() const;
    GameItems *items() const;
    GameItems *chests() const;
    GameItems *enemies() const;
    GameItems *characters() const;
    WeatherAreaModel *weatherAreas() const;

    void loadMap(const QString &resourcePath);
    void loadMapVariant(const QVariantMap &mapData);

    Q_INVOKABLE Field *getField(int x, int y) const;
    Q_INVOKABLE Field *getField(const QPointF position) const;

private:
    QSize m_size;
    QPointF m_playerStartPosition;
    QString m_name;
    QString m_resourcePath;
    QString m_resourceFileName;
    QColor m_backgroundColor;

    Character *m_player = nullptr;
    GameObjects *m_objects = nullptr;
    GameItems *m_items = nullptr;
    GameItems *m_chests = nullptr;
    GameItems *m_enemies = nullptr;
    GameItems *m_characters = nullptr;
    WeatherAreaModel *m_weatherAreaModel = nullptr;

    QList<Fields *> m_mapData;

    void placeItemOnMap(GameItem *item);

signals:
    void sizeChanged(const QSize &size);
    void playerStartPositionChanged(const QPointF &playerStartPosition);
    void nameChanged(const QString &name);
    void resourceFileNameChanged(const QString &resourceFileName);
    void backgroundColorChanged(const QColor &backgroundColor);

public slots:
    void clear();

};

QDebug operator<<(QDebug debug, Map *map);

#endif // MAP_H
