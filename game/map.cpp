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
    QVariantMap mapData = loadMapData(fileName);
    if (mapData.isEmpty()) {
        qCWarning(dcMap()) << "The map file" << fileName << "does not contain any valid map data.";
        return;
    }

    setSize(QSize(mapData.value("width").toInt(), mapData.value("height").toInt()));
    setName(mapData.value("name").toString());
    setPlayerStartPosition(QPointF(mapData.value("playerStartX").toDouble(), mapData.value("playerStartY").toDouble()));

    // Default field information
    QVariantMap defaultFieldMap = mapData.value("defaultField").toMap();
    bool defaultAccessable = defaultFieldMap.value("accessable").toBool();
    QString defaultImageName = defaultFieldMap.value("imageName").toString();

    // Initialize fields
    qCDebug(dcMap()) << "--> initialize fields";
    for (int y = 0; y < m_size.height(); y++) {
        Fields *fields = new Fields();
        for (int x = 0; x < m_size.width(); x++) {
            // Create field for x,y coordinate and init with default
            Field *field = new Field(QPoint(x, y), fields);
            field->setAccessible(defaultAccessable);
            field->setImageName(defaultImageName);
            //qCDebug(dcMap()) << "Map: create field" << field->position();
            fields->addField(field);
        }
        m_mapData.append(fields);
    }

    qCDebug(dcMap()) << "--> initialize field neighbours";
    // Set field neighbours
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
            qWarning() << "Ignore field form map data" << fieldMap;
            continue;
        }

        field->setAccessible(fieldMap.value("accessable").toBool());
        field->setImageName(fieldMap.value("imageName").toString());
    }

    qCDebug(dcMap()) << "--> load items";

    // Load plants
    qCDebug(dcMap()) << "    --> load plants";
    QVariantList plantsList = mapData.value("items").toMap().value("plants").toList();
    foreach (const QVariant &plantVariant, plantsList) {
        PlantItem *plantItem = createPlantItem(plantVariant.toMap());
        if (!plantItem) {
            qCWarning(dcMap()) << "Could not create plant item for" << plantVariant;
            continue;
        }

        // Place the item into the field
        Field *field = getField(plantItem->position().x(), plantItem->position().y());
        if (!field) {
            qCWarning(dcMap()) << "Could not find any field for" << plantItem;
            plantItem->deleteLater();
            continue;
        }
        field->setGameItem(plantItem);

        qCDebug(dcMap()) << "        " << plantItem;
        plantItem->moveToThread(QCoreApplication::instance()->thread());
        m_plants.append(plantItem);
    }

    qCDebug(dcMap()) << "    --> load trees";
    QVariantList treesList = mapData.value("items").toMap().value("trees").toList();
    foreach (const QVariant &treeVariant, treesList) {
        TreeItem *treeItem = createTreeItem(treeVariant.toMap());
        if (!treeItem) {
            qCWarning(dcMap()) << "Could not create tree item for" << treeVariant;
            continue;
        }

        // Place the item into the field
        Field *field = getField(treeItem->position().x(), treeItem->position().y());
        if (!field) {
            qCWarning(dcMap()) << "Could not find any field for" << treeItem;
            treeItem->deleteLater();
            continue;
        }
        field->setGameItem(treeItem);

        qCDebug(dcMap()) << "        " << treeItem;
        treeItem->moveToThread(QCoreApplication::instance()->thread());
        m_trees.append(treeItem);
    }

    // Move all the object back to the main thread
    foreach (Fields *fields, m_mapData) {
        fields->moveToThread(QCoreApplication::instance()->thread());
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

QVariantMap Map::loadMapData(const QString &mapDataFileName)
{
    QFile mapDataFile(mapDataFileName);
    if (!mapDataFile.open(QFile::ReadOnly)) {
        qCWarning(dcMap()) << "Could not open map data file" << mapDataFileName << "for reading.";
        return QVariantMap();
    }

    QJsonParseError error;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(mapDataFile.readAll(), &error);
    if (error.error != QJsonParseError::NoError) {
        qCWarning(dcMap()) << "Cannot parse map data file:" << error.errorString();
        return QVariantMap();
    }

    return jsonDoc.toVariant().toMap();
}

PlantItem *Map::createPlantItem(const QVariantMap &plantItemMap)
{
    QPoint position = QPoint(plantItemMap.value("x").toInt(), plantItemMap.value("y").toInt());
    QString itemDescriptionFileName = plantItemMap.value("plant").toString();

    QVariantMap description = loadMapData(itemDescriptionFileName);
    if (description.isEmpty()) {
        qCWarning(dcMap()) << "The plant file" << itemDescriptionFileName << "does not contains any valid data.";
        return nullptr;
    }

    QVariantMap propertiesMap = description.value("properties").toMap();
    QSize itemSize(propertiesMap.value("width", 1).toInt(), propertiesMap.value("height", 1).toInt());

    PlantItem *plantItem = new PlantItem();
    plantItem->setPosition(position);
    plantItem->setSize(itemSize);
    plantItem->setName(description.value("name").toString());
    plantItem->setImageName(description.value("imageName").toString());
    plantItem->setPrice(description.value("price").toInt());
    plantItem->setHealing(description.value("healing").toInt());
    plantItem->setMana(description.value("mana").toInt());
    plantItem->setSpeed(description.value("speed").toInt());
    return plantItem;
}

TreeItem *Map::createTreeItem(const QVariantMap &treeItemMap)
{
    QPoint position = QPoint(treeItemMap.value("x").toInt(), treeItemMap.value("y").toInt());
    QString itemDescriptionFileName = treeItemMap.value("tree").toString();

    QVariantMap description = loadMapData(itemDescriptionFileName);
    if (description.isEmpty()) {
        qCWarning(dcMap()) << "The tree file" << itemDescriptionFileName << "does not contains any valid data.";
        return nullptr;
    }

    QVariantMap propertiesMap = description.value("properties").toMap();
    QSize itemSize(propertiesMap.value("width", 1).toInt(), propertiesMap.value("height", 1).toInt());

    TreeItem *treeItem = new TreeItem();
    treeItem->setPosition(position);
    treeItem->setSize(itemSize);
    treeItem->setName(description.value("name").toString());
    treeItem->setImageName(description.value("imageName").toString());

    return treeItem;
}

void Map::clear()
{
    // Delete all fields
    qDeleteAll(m_mapData);
    m_mapData.clear();
}
