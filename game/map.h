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
    Q_PROPERTY(qreal ambientBrightness READ ambientBrightness WRITE setAmbientBrightness NOTIFY ambientBrightnessChanged)
    Q_PROPERTY(QColor ambientLightColor READ ambientLightColor WRITE setAmbientLightColor NOTIFY ambientLightColorChanged)
    Q_PROPERTY(qreal grayscaleFactor READ grayscaleFactor WRITE setGrayscaleFactor NOTIFY grayscaleFactorChanged)

public:
    explicit Map(QObject *parent = nullptr);
    Map(GameObjects *backgroundLights, GameObjects *objects, GameObjects *lights, GameItems *items, GameItems *enemies, GameItems *characters, QObject *parent = nullptr);
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

    // Ambient information
    QColor backgroundColor() const;
    void setBackgroundColor(const QColor &backgroundColor);

    qreal ambientBrightness() const;
    void setAmbientBrightness(qreal ambientBrightness);

    QColor ambientLightColor() const;
    void setAmbientLightColor(const QColor &ambientLightColor);

    qreal grayscaleFactor() const;
    void setGrayscaleFactor(qreal grayscaleFactor);


    // Weather information
    bool raining() const;
    void setRaining(bool raining);

    bool snowing() const;
    void setSnowing(bool snowing);


    Character *player() const;
    void setPlayer(Character *player);

    GameObjects *objects() const;
    GameObjects *backgroundLights() const;
    GameObjects *lights() const;
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
    qreal m_ambientBrightness = 1.0;
    QColor m_ambientLightColor = Qt::transparent;
    qreal m_grayscaleFactor = 0.0;

    bool m_raining = false;
    bool m_snowing = false;

    Character *m_player = nullptr;

    GameObjects *m_objects = nullptr;
    GameObjects *m_backgroundLights = nullptr;
    GameObjects *m_lights = nullptr;
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
    void ambientBrightnessChanged(qreal ambientBrightness);
    void ambientLightColorChanged(const QColor &ambientLightColor);
    void grayscaleFactorChanged(qreal grayscaleFactor);

public slots:
    void clear();

};

QDebug operator<<(QDebug debug, Map *map);

#endif // MAP_H
