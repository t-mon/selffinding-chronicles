#include "map.h"
#include "debugcategories.h"

#include <QTime>
#include <QFile>
#include <QJsonDocument>
#include <QElapsedTimer>
#include <QJsonParseError>
#include <QCoreApplication>

Map::Map(QObject *parent) : QObject(parent)
{
    m_items = new GameItems(this);
    m_enemies = new GameItems(this);
    m_characters = new GameItems(this);
}

Map::Map(GameItems *items, GameItems *enemies, GameItems *characters, QObject *parent) :
    QObject(parent)
{
    m_items = items;
    m_enemies = enemies;
    m_characters = characters;
}

Map::~Map()
{

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

QString Map::fileName() const
{
    return m_fileName;
}

QColor Map::backgroundColor() const
{
    return m_backgroundColor;
}

void Map::setBackgroundColor(const QColor &backgroundColor)
{
    if (m_backgroundColor == backgroundColor)
        return;

    m_backgroundColor = backgroundColor;
    emit backgroundColorChanged(m_backgroundColor);
}

GameItems *Map::items()
{
    return m_items;
}

GameItems *Map::enemies()
{
    return m_enemies;
}

GameItems *Map::characters()
{
    return m_characters;
}

void Map::loadMap(const QString &fileName)
{
    qCDebug(dcMap()) << "Start loading map from" << fileName;

    if (m_fileName == fileName) {
        qCDebug(dcMap()) << "The map" << fileName << "is already loaded.";
        return;
    }

    m_fileName = fileName;
    emit fileNameChanged(m_fileName);

    // Load map information
    qCDebug(dcMap()) << "--> load json data from file";
    QVariantMap mapData = DataLoader::loadJsonData(fileName);
    if (mapData.isEmpty()) {
        qCWarning(dcMap()) << "The map file" << fileName << "does not contain any valid map data.";
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

    // Load items
    qCDebug(dcMap()) << "--> load items" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> items = DataLoader::loadGameItems(mapData.value("items").toList(), this);
    m_items->addGameItemList(items);
    foreach (GameItem *item, items)
        qCDebug(dcMap()) << "        " << item;

    // Load chests
    qCDebug(dcMap()) << "--> load chests" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> chestItems = DataLoader::loadChestItems(mapData.value("chests").toList(), this);
    m_items->addGameItemList(chestItems);
    foreach (GameItem *item, chestItems)
        qCDebug(dcMap()) << "        " << item;

    // Load characters
    qCDebug(dcMap()) << "--> load characters" << QDateTime::currentMSecsSinceEpoch() - startTime << "[ms]";
    QList<GameItem *> characterItems = DataLoader::loadCharacterItems(mapData.value("characters").toList(), this);
    m_characters->addGameItemList(characterItems);
    foreach (GameItem *item, m_characters->gameItems()) {
        Character *characterItem = qobject_cast<Character *>(item);
        if (!characterItem->paths().isEmpty()) {
            characterItem->pathController()->setPath(characterItem->paths().first(), characterItem->position());
        }
        qCDebug(dcMap()) << "        " << item;
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
        qCDebug(dcMap()) << "        " << item;
    }

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
    m_fileName = QString();
    m_backgroundColor = QColor();

    // Delete all objects of the map
    m_items->clearModel();
    m_characters->clearModel();
    m_enemies->clearModel();

    // Delete all fields
    qDeleteAll(m_mapData);
    m_mapData.clear();

}

QDebug operator<<(QDebug debug, Map *map)
{
    debug.nospace() << "Map("<< map->name();
    debug.nospace() << ", " << map->size();
    debug.nospace() << ", " << map->fileName();
    debug.nospace() << ", " << map->playerStartPosition();
    debug.nospace() << ", color: " << map->backgroundColor().name();
    debug.nospace() << ")" << endl;

    debug.nospace() << "--> Items:" << endl;
    foreach (GameItem *item, map->items()->gameItems()) {

        switch (item->itemType()) {
        case GameItem::TypeChest:
            debug.nospace() << "    " << item->thread() << ", " << qobject_cast<ChestItem *>(item) << endl;
            break;
        default:
            debug.nospace() << "    " << item->thread() << ", " << item << endl;
            break;
        }

    }

    debug.nospace() << "--> Characters:" << endl;
    foreach (GameItem *item, map->characters()->gameItems()) {
        debug.nospace() << "    " << item->thread() << ", " << qobject_cast<Character *>(item) << endl;
    }

    debug.nospace() << "--> Enemies:" << endl;
    foreach (GameItem *item, map->enemies()->gameItems()) {
        debug.nospace() << "    " << item->thread() << ", " << item << endl;
    }

    return debug.space();
}
