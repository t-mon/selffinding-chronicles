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
    m_items->setAutoParent(true);

    m_enemies = new GameItems(this);
    m_enemies->setAutoParent(true);

    m_characters = new GameItems(this);
    m_enemies->setAutoParent(true);
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

QString Map::fileName() const
{
    return m_fileName;
}

GameItems *Map::items()
{
    return m_items;
}

GameItems *Map::characters()
{
    return m_characters;
}

void Map::loadMap(const QString &fileName)
{
    QElapsedTimer timer;
    qCDebug(dcMap()) << "Start loading map from" << fileName;
    timer.start();

    if (m_fileName == fileName) {
        qCDebug(dcMap()) << "The map" << fileName << "is already loaded.";
        return;
    }

    m_fileName = fileName;

    qCDebug(dcMap()) << "--> clean up old map data";
    clear();

    // Load map information
    qCDebug(dcMap()) << "--> load json data from file";
    QVariantMap mapData = DataLoader::loadJsonData(fileName);
    if (mapData.isEmpty()) {
        qCWarning(dcMap()) << "The map file" << fileName << "does not contain any valid map data.";
        return;
    }

    setSize(QSize(mapData.value("width").toInt(), mapData.value("height").toInt()));
    setName(mapData.value("name").toString());
    setPlayerStartPosition(QPointF(mapData.value("playerStartX").toDouble(), mapData.value("playerStartY").toDouble()));

    // Default field information
    QVariantMap defaultFieldMap = mapData.value("defaultField").toMap();
    QString defaultImageName = defaultFieldMap.value("imageName").toString();

    // Initialize fields
    qCDebug(dcMap()) << "--> initialize fields";
    for (int y = 0; y < m_size.height(); y++) {
        Fields *fields = new Fields();
        for (int x = 0; x < m_size.width(); x++) {
            // Create field for x,y coordinate and init with default
            Field *field = new Field(QPoint(x, y), fields);
            field->setImageName(defaultImageName);
            //qCDebug(dcMap()) << "Map: create field" << field->position();
            fields->addField(field);
        }
        m_mapData.append(fields);
    }

    // Set field neighbours
    qCDebug(dcMap()) << "--> initialize field neighbours";
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

    // Set fields data
    qCDebug(dcMap()) << "--> configure fields";
    QVariantList fieldList = mapData.value("fields").toList();
    foreach (const QVariant &fieldVariant, fieldList) {
        QVariantMap fieldMap = fieldVariant.toMap();
        int x = fieldMap.value("x").toInt();
        int y = fieldMap.value("y").toInt();

        Field *field = getField(x, y);
        if (!field) {
            qCWarning(dcMap()) << "Ignore field form map data" << fieldMap;
            continue;
        }

        //field->setAccessible(fieldMap.value("accessable").toBool());
        field->setImageName(fieldMap.value("imageName").toString());
    }

    qCDebug(dcMap()) << "--> load items";
    QList<GameItem *> items = DataLoader::loadGameItems(mapData.value("items").toList());
    foreach (GameItem *item, items)
        item->moveToThread(QCoreApplication::instance()->thread());

    m_items->addGameItemList(items);


    qCDebug(dcMap()) << "--> load chests";
    QList<GameItem *> chestItems = DataLoader::loadGameItems(mapData.value("chests").toList());
    foreach (GameItem *chestItem, chestItems)
        chestItem->moveToThread(QCoreApplication::instance()->thread());

    m_items->addGameItemList(chestItems);


    qCDebug(dcMap()) << "--> load characters";
    QList<GameItem *> characterItems = DataLoader::loadGameItems(mapData.value("characters").toList());
    foreach (GameItem *characterItem, characterItems)
        characterItem->moveToThread(QCoreApplication::instance()->thread());

    m_characters->addGameItemList(characterItems);

    qCDebug(dcMap()) << "--> load enemies";

    QList<GameItem *> enemyItems = DataLoader::loadGameItems(mapData.value("enemies").toList());
    foreach (GameItem *enemyItem, enemyItems)
            enemyItem->moveToThread(QCoreApplication::instance()->thread());

    m_enemies->addGameItemList(enemyItems);

    // Move all the object back to the main thread
    foreach (Fields *fields, m_mapData)
        fields->moveToThread(QCoreApplication::instance()->thread());

    foreach (GameItem *item, m_items->gameItems()) {
        item->moveToThread(QCoreApplication::instance()->thread());
        placeItemOnMap(item);
        qCDebug(dcMap()) << "        " << item;
    }

    foreach (GameItem *item, m_characters->gameItems()) {
        item->moveToThread(QCoreApplication::instance()->thread());
        placeItemOnMap(item);
        qCDebug(dcMap()) << "        " << item;
    }

    foreach (GameItem *item, m_enemies->gameItems()) {
        item->moveToThread(QCoreApplication::instance()->thread());
        placeItemOnMap(item);
        qCDebug(dcMap()) << "        " << item;
    }

    qCDebug(dcMap()) << "Map loading from" << fileName << "finished successfully:" << timer.elapsed() << "[ms]";
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
    // Delete all fields
    qDeleteAll(m_mapData);
    m_mapData.clear();
}
