#include "map.h"
#include "debugcategories.h"

#include <QTime>
#include <QFile>
#include <QFileInfo>
#include <QJsonDocument>
#include <QElapsedTimer>
#include <QJsonParseError>
#include <QCoreApplication>

Map::Map(QObject *parent) : QObject(parent)
{
    m_backgroundLights = new GameObjects(this);
    m_objects = new GameObjects(this);
    m_lights = new GameObjects(this);
    m_items = new GameItems(this);
    m_chests = new GameItems(this);
    m_enemies = new GameItems(this);
    m_characters = new GameItems(this);
    m_weatherAreaModel = new WeatherAreaModel(this);
}

Map::Map(GameObjects *backgroundLights, GameObjects *objects, GameObjects *lights, GameItems *items, GameItems *enemies, GameItems *characters, QObject *parent) :
    QObject(parent)
{
    m_backgroundLights = backgroundLights;
    m_objects = objects;
    m_lights = lights;
    m_items = items;
    m_enemies = enemies;
    m_characters = characters;
}

Map::~Map()
{
    clear();
}

QSize Map::size() const
{
    return m_size;
}

void Map::setSize(const QSize &size)
{
    if (m_size == size)
        return;

    m_size = size;
    emit sizeChanged(m_size);
}

QPointF Map::playerStartPosition() const
{
    return m_playerStartPosition;
}

void Map::setPlayerStartPosition(const QPointF &playerStartPosition)
{
    if (m_playerStartPosition == playerStartPosition)
        return;

    m_playerStartPosition = playerStartPosition;
    emit playerStartPositionChanged(m_playerStartPosition);
}

QString Map::name() const
{
    return m_name;
}

void Map::setName(const QString &name)
{
    if (m_name == name)
        return;

    m_name = name;
    emit nameChanged(m_name);
}

QString Map::resourcePath() const
{
    return m_resourcePath;
}

void Map::setResourcePath(const QString &resourcePath)
{
    if (m_resourcePath == resourcePath)
        return;

    m_resourcePath = resourcePath;
    m_resourceFileName = QFileInfo(m_resourcePath).fileName();
    emit resourceFileNameChanged(m_resourceFileName);
}

QString Map::resourceFileName() const
{
    return m_resourceFileName;
}

QColor Map::backgroundColor() const
{
    return m_backgroundColor;
}

void Map::setBackgroundColor(const QColor &backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    qCDebug(dcMap()) << "Background color changed" << backgroundColor.name();
    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(m_backgroundColor);
}

qreal Map::ambientBrightness() const
{
    return m_ambientBrightness;
}

void Map::setAmbientBrightness(qreal ambientBrightness)
{
    if (m_ambientBrightness == ambientBrightness)
        return;

    m_ambientBrightness = ambientBrightness;
    emit ambientBrightnessChanged(m_ambientBrightness);
}

QColor Map::ambientLightColor() const
{
    return m_ambientLightColor;
}

void Map::setAmbientLightColor(const QColor &ambientLightColor)
{
    if (m_ambientLightColor == ambientLightColor)
        return;

    m_ambientLightColor = ambientLightColor;
    emit ambientLightColorChanged(m_ambientLightColor);
}

qreal Map::grayscaleFactor() const
{
    return m_grayscaleFactor;
}

void Map::setGrayscaleFactor(qreal grayscaleFactor)
{
    if (m_grayscaleFactor == grayscaleFactor)
        return;

    m_grayscaleFactor = grayscaleFactor;
    emit grayscaleFactorChanged(m_grayscaleFactor);
}

Character *Map::player() const
{
    return m_player;
}

void Map::setPlayer(Character *player)
{
    m_player = player;
    m_player->setIsPlayer(true);

    // Note: add the player always to the character list
    if (m_player) {
        m_characters->addGameItem(m_player);

        if (m_player->lightSource()) {
            m_lights->addGameObject(m_player->lightSource());
        }
    }
}

GameObjects *Map::objects() const
{
    return m_objects;
}

GameObjects *Map::backgroundLights() const
{
    return m_backgroundLights;
}

GameObjects *Map::lights() const
{
    return m_lights;
}

GameItems *Map::items() const
{
    return m_items;
}

GameItems *Map::chests() const
{
    return m_chests;
}

GameItems *Map::enemies() const
{
    return m_enemies;
}

GameItems *Map::characters() const
{
    return m_characters;
}

WeatherAreaModel *Map::weatherAreas() const
{
    return m_weatherAreaModel;
}

void Map::loadMap(const QString &resourcePath)
{
    qCDebug(dcMap()) << "Start loading map from" << resourcePath;

    if (m_resourcePath == resourcePath) {
        qCDebug(dcMap()) << "The map" << resourcePath << "is already loaded.";
        return;
    }

    setResourcePath(resourcePath);

    // Load map information
    qCDebug(dcMap()) << "--> load json data from file" << resourcePath;
    QVariantMap mapData = DataLoader::loadJsonData(resourcePath);
    if (mapData.isEmpty()) {
        qCWarning(dcMap()) << "The map file" << resourcePath << "does not contain any valid map data.";
        return;
    }

    loadMapVariant(mapData);
}

void Map::loadMapVariant(const QVariantMap &mapData)
{
    qint64 startTime = QDateTime::currentMSecsSinceEpoch();

    setSize(QSize(mapData.value("width").toInt(), mapData.value("height").toInt()));
    setName(mapData.value("name").toString());
    setPlayerStartPosition(QPointF(mapData.value("playerStartX").toDouble(), mapData.value("playerStartY").toDouble()));
    setBackgroundColor(QColor(mapData.value("backgroundColor").toString()));
    setAmbientLightColor(QColor(mapData.value("ambientLightColor", QColor("#00000000")).toString()));
    setAmbientBrightness(mapData.value("ambientBrightness", 1.0).toDouble());
    setGrayscaleFactor(mapData.value("grayscaleFactor", 0.0).toDouble());

    // Initialize fields
    qCDebug(dcMap()) << "--> initialize fields" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    for (int y = 0; y < m_size.height(); y++) {
        Fields *fields = new Fields(this);
        for (int x = 0; x < m_size.width(); x++) {
            // Create field for x,y coordinate and init with default
            Field *field = new Field(QPoint(x, y), fields);
            fields->addField(field);
        }
        m_mapData.append(fields);
    }

    // Set field neighbours
    qCDebug(dcMap()) << "--> initialize field neighbours" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    for (int y = 0; y < m_size.height(); y++) {
        for (int x = 0; x < m_size.width(); x++) {
            Field *currentField = getField(x, y);
            currentField->setNorthField(getField(x, y - 1));
            currentField->setNorthEastField(getField(x + 1, y - 1));
            currentField->setEastField(getField(x + 1, y));
            currentField->setSouthEastField(getField(x + 1, y + 1));
            currentField->setSouthField(getField(x, y + 1));
            currentField->setSouthWestField(getField(x - 1, y + 1));
            currentField->setWestField(getField(x - 1, y));
            currentField->setNorthWestField(getField(x - 1, y - 1));
        }
    }

    // Load objects
    qCDebug(dcMap()) << "--> load objects" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameObject *> objects = DataLoader::loadGameObjects(mapData.value("objects").toList(), this);
    m_objects->addGameObjectList(objects);
    foreach (GameObject *object, objects) {
        qCDebug(dcMap()) << "        " << object;
    }

    // Load items
    qCDebug(dcMap()) << "--> load items" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> items = DataLoader::loadGameItems(mapData.value("items").toList(), this);
    m_items->addGameItemList(items);
    foreach (GameItem *item, items) {
        qCDebug(dcMap()) << "        " << item;
        if (item->lightSource()) {
            qCDebug(dcMap()) << "        " << qobject_cast<GameObject *>(item->lightSource());
            m_lights->addGameObject(qobject_cast<GameObject *>(item->lightSource()));
        }
    }

    // Load chests
    qCDebug(dcMap()) << "--> load chests" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> chestItems = DataLoader::loadChestItems(mapData.value("chests").toList(), this);
    m_chests->addGameItemList(chestItems);
    foreach (GameItem *item, chestItems) {
        qCDebug(dcMap()) << "        " << qobject_cast<ChestItem *>(item);
        if (item->lightSource()) {
            m_lights->addGameObject(qobject_cast<GameObject *>(item->lightSource()));
            qCDebug(dcMap()) << "        " << qobject_cast<GameObject *>(item->lightSource());
        }
    }

    // Load characters
    qCDebug(dcMap()) << "--> load characters" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> characterItems = DataLoader::loadCharacterItems(mapData.value("characters").toList(), this);
    m_characters->addGameItemList(characterItems);
    foreach (GameItem *item, m_characters->gameItems()) {
        Character *characterItem = qobject_cast<Character *>(item);
        if (!characterItem->paths().isEmpty()) {
            characterItem->pathController()->setPath(characterItem->paths().first(), characterItem->position());
        }
        qCDebug(dcMap()) << "        " << qobject_cast<Character *>(item);
        if (item->lightSource()) {
            m_lights->addGameObject(qobject_cast<GameObject *>(item->lightSource()));
            qCDebug(dcMap()) << "        " << qobject_cast<GameObject *>(item->lightSource());
        }
    }

    // Load enemies
    qCDebug(dcMap()) << "--> load enemies" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> enemyItems = DataLoader::loadEnemyItems(mapData.value("enemies").toList(), this);
    m_enemies->addGameItemList(enemyItems);
    foreach (GameItem *item, m_enemies->gameItems()) {
        Character *characterItem = qobject_cast<Character *>(item);
        if (!characterItem->paths().isEmpty()) {
            characterItem->pathController()->setPath(characterItem->paths().first(), characterItem->position());
        }
        qCDebug(dcMap()) << "        " << qobject_cast<Enemy *>(item);
        if (item->lightSource()) {
            m_lights->addGameObject(item->lightSource());
            qCDebug(dcMap()) << "        " << qobject_cast<GameObject *>(item->lightSource());
        }
    }

    // Create weater areas for the entire map
    qCDebug(dcMap()) << "Initialize weather areas for map size" << m_size;
    m_weatherAreaModel->initializeAreas(m_size);
    qCDebug(dcMap()) << "Weather area total count for world size" << m_size << "Count:" << m_weatherAreaModel->rowCount();
    qCDebug(dcMap()) << "Map loading finished successfully:" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
}

Field *Map::getField(int x, int y) const
{
    if (y < 0 || y >= m_mapData.count())
        return nullptr;

    if (x < 0 || x >= m_mapData.at(y)->rowCount())
        return nullptr;

    return m_mapData.at(y)->get(x);
}

Field *Map::getField(const QPointF position) const
{
    return getField(static_cast<int>(position.x()), static_cast<int>(position.y()));
}

void Map::placeItemOnMap(GameItem *item)
{
    Field *field = getField(item->position());
    if (!field)
        return;

    field->gameItems()->addGameItem(item);
}

void Map::clear()
{
    m_size = QSize();
    m_playerStartPosition = QPointF();
    m_name = QString();
    m_resourcePath = QString();
    m_resourceFileName = QString();
    m_backgroundColor = QColor();

    // Delete all objects of the map
    m_player = nullptr;
    m_objects->clearModel();
    m_items->clearModel();
    m_chests->clearModel();
    m_characters->clearModel();
    m_enemies->clearModel();
    m_lights->clearModel();

    // Delete all fields
    qDeleteAll(m_mapData);
    m_mapData.clear();

}

QDebug operator<<(QDebug debug, Map *map)
{
    debug.nospace() << "Map("<< map->name();
    debug.nospace() << ", " << map->size();
    debug.nospace() << ", " << map->resourcePath();
    debug.nospace() << ", " << map->resourceFileName();
    debug.nospace() << ", Start position:" << map->playerStartPosition();
    debug.nospace() << ", color: " << map->backgroundColor().name();
    debug.nospace() << ")" << Qt::endl;

    debug.nospace() << "--> Objects:" << Qt::endl;
    foreach (GameObject *object, map->objects()->gameObjects()) {
        debug.nospace() << "    " << object << Qt::endl;
    }

    debug.nospace() << "--> Lights:" << Qt::endl;
    foreach (GameObject *object, map->lights()->gameObjects()) {
        debug.nospace() << "    " << object << Qt::endl;
    }

    debug.nospace() << "--> Items:" << Qt::endl;
    foreach (GameItem *item, map->items()->gameItems()) {
        debug.nospace() << "    " << item << Qt::endl;
    }

    debug.nospace() << "--> Chests:" << Qt::endl;
    foreach (GameItem *item, map->chests()->gameItems()) {
        debug.nospace() << "    " << qobject_cast<ChestItem *>(item) << Qt::endl;
    }

    debug.nospace() << "--> Characters:" << Qt::endl;
    foreach (GameItem *item, map->characters()->gameItems()) {
        debug.nospace() << "    " << qobject_cast<Character *>(item) << Qt::endl;
    }

    debug.nospace() << "--> Enemies:" << Qt::endl;
    foreach (GameItem *item, map->enemies()->gameItems()) {
        debug.nospace() << "    " << qobject_cast<Enemy *>(item) << Qt::endl;
    }

    return debug.space();
}
