#include "gamemapeditor.h"
#include "debugcategories.h"
#include "dataloader.h"
#include "game.h"

GameMapEditor::GameMapEditor(QObject *parent) :
    QObject(parent)
{
    qRegisterMetaType<GameMapEditor::Tool>("Tool");

    m_availableItems = new GameItems(this);
    m_availableObjects = new GameObjects(this);
    m_availableCharacters = new GameItems(this);
    m_availableEnemies = new GameItems(this);

    m_dataManager = new DataManager(this);
    connect(m_dataManager, &DataManager::stateChanged, this, &GameMapEditor::onDataManagerStateChanged);

    m_mapScene = new MapScene(this);

    qCDebug(dcMapEditor()) << "Instance created";
}

Map *GameMapEditor::map() const
{
    return m_map;
}

MapScene *GameMapEditor::mapScene() const
{
    return m_mapScene;
}

DataManager *GameMapEditor::dataManager() const
{
    return m_dataManager;
}

GameObjects *GameMapEditor::availableObjects() const
{
    return m_availableObjects;
}

GameItems *GameMapEditor::availableItems() const
{
    return m_availableItems;
}

GameItems *GameMapEditor::availableCharacters() const
{
    return m_availableCharacters;
}

GameItems *GameMapEditor::availableEnemies() const
{
    return m_availableEnemies;
}

void GameMapEditor::loadAvailableGameItems()
{
    if (!m_availableItems->gameItems().isEmpty()) {
        qCDebug(dcMapEditor()) << "Available items already loaded. Doing nothing";
        return;
    }

    qCDebug(dcMapEditor()) << "Loading available items...";

    QString gameDataPath = ":/gamedata/items/";
    QDir resourceDirectory(gameDataPath);
    foreach (const QString &gameDataEntry, resourceDirectory.entryList()) {
        qCDebug(dcMapEditor()) << "--> Resource entry" << QString(gameDataPath + gameDataEntry);
        QDir typeDirectory(gameDataPath + gameDataEntry);
        foreach (const QString &entry, typeDirectory.entryList()) {
            qCDebug(dcMapEditor()) << "          --> entry" << typeDirectory.path() + "/" + entry;
            m_availableItems->addGameItem(DataLoader::loadGameItemFromResourcePath(typeDirectory.path() + "/" + entry, this));
        }
    }
}

void GameMapEditor::loadAvailableObjects()
{
    if (!m_availableObjects->gameObjects().isEmpty()) {
        qCDebug(dcMapEditor()) << "Available objects already loaded. Doing nothing";
        return;
    }

    qCDebug(dcMapEditor()) << "Loading available objects...";

    QString gameDataPath = ":/gamedata/objects/";
    QDir resourceDirectory(gameDataPath);
    foreach (const QString &gameDataEntry, resourceDirectory.entryList()) {
        qCDebug(dcMapEditor()) << "--> Resource entry" << QString(gameDataPath + gameDataEntry);
        QDir typeDirectory(gameDataPath + gameDataEntry);
        foreach (const QString &entry, typeDirectory.entryList()) {
            qCDebug(dcMapEditor()) << "          --> entry" << typeDirectory.path() + "/" + entry;
            m_availableObjects->addGameObject(DataLoader::loadGameObjectFromResourcePath(typeDirectory.path() + "/" + entry, this));
        }
    }
}

void GameMapEditor::loadAvailableCharacters()
{
    if (!m_availableCharacters->gameItems().isEmpty()) {
        qCDebug(dcMapEditor()) << "Available characters already loaded. Doing nothing";
        return;
    }

    qCDebug(dcMapEditor()) << "Loading available characters...";

    QString gameDataPath = ":/gamedata/characters/";
    QDir resourceDirectory(gameDataPath);
    foreach (const QString &gameDataEntry, resourceDirectory.entryList()) {
        qCDebug(dcMapEditor()) << "--> Resource entry" << QString(gameDataPath + gameDataEntry);
        QDir typeDirectory(gameDataPath + gameDataEntry);
        foreach (const QString &entry, typeDirectory.entryList()) {
            qCDebug(dcMapEditor()) << "          --> entry" << typeDirectory.path() + "/" + entry;
            m_availableCharacters->addGameItem(DataLoader::loadGameItemFromResourcePath(typeDirectory.path() + "/" + entry, this));
        }
    }
}

void GameMapEditor::loadAvailableEnemies()
{
    if (!m_availableEnemies->gameItems().isEmpty()) {
        qCDebug(dcMapEditor()) << "Available enemies already loaded. Doing nothing";
        return;
    }

    qCDebug(dcMapEditor()) << "Loading available enemies...";

    QString gameDataPath = ":/gamedata/enemies/";
    QDir resourceDirectory(gameDataPath);
    foreach (const QString &gameDataEntry, resourceDirectory.entryList()) {
        qCDebug(dcMapEditor()) << "--> Resource entry" << QString(gameDataPath + gameDataEntry);
        QDir typeDirectory(gameDataPath + gameDataEntry);
        foreach (const QString &entry, typeDirectory.entryList()) {
            qCDebug(dcMapEditor()) << "          --> entry" << typeDirectory.path() + "/" + entry;
            m_availableEnemies->addGameItem(DataLoader::loadGameItemFromResourcePath(typeDirectory.path() + "/" + entry, this));
        }
    }
}

GameMapEditor::Tool GameMapEditor::tool() const
{
    return m_tool;
}

void GameMapEditor::setTool(GameMapEditor::Tool tool)
{
    if (m_tool == tool)
        return;

    qCDebug(dcMapEditor()) << "Tool changed" << tool;
    m_tool = tool;
    emit toolChanged(m_tool);
}

GameMapEditor::Mode GameMapEditor::mode() const
{
    return m_mode;
}

void GameMapEditor::setMode(GameMapEditor::Mode mode)
{
    if (m_mode == mode)
        return;

    qCDebug(dcMapEditor()) << "Mode changed" << mode;
    m_mode = mode;
    emit modeChanged(m_mode);
}

GameItem *GameMapEditor::selectedGameItem() const
{
    return m_selectedGameItem;
}

void GameMapEditor::createSelectedGameItem(const QString &resourcePath)
{
    if (m_selectedGameItem)
        m_selectedGameItem->deleteLater();

    qCDebug(dcMapEditor()) << "Create selected item" << resourcePath;
    m_selectedGameItem = DataLoader::loadGameItemFromResourcePath(resourcePath, this);
    emit selectedGameItemChanged(m_selectedGameItem);
}

GameObject *GameMapEditor::selectedGameObject() const
{
    return m_selectedGameObject;
}

void GameMapEditor::createSelectedGameObject(const QString &resourcePath)
{
    if (m_selectedGameObject)
        m_selectedGameObject->deleteLater();

    qCDebug(dcMapEditor()) << "Create selected object" << resourcePath;
    m_selectedGameObject = DataLoader::loadGameObjectFromResourcePath(resourcePath, this);
    emit selectedGameObjectChanged(m_selectedGameObject);
}

void GameMapEditor::createNewMap(const QString &mapName, const QSize &mapSize)
{
    qCDebug(dcMapEditor()) << "Create new map" << mapName << mapSize;
    if (m_map) {
        qCWarning(dcMapEditor()) << "Deleting current loaded map. FIXME: check if autosave";
        m_map->deleteLater();
        m_map = nullptr;
    }

    m_map = new Map(this);
    m_map->setName(mapName);
    m_map->setSize(mapSize);

    m_mapScene->setMap(m_map);
}

void GameMapEditor::placeItemOnMap(const QString &resourcePath, const QPointF &position)
{
    qCDebug(dcMapEditor()) << "Place item" << resourcePath << position;
    GameItem *item = DataLoader::loadGameItemFromResourcePath(resourcePath, m_map);
    item->setPosition(position);

    if (item->itemType() == GameItem::TypeCharacter) {
        m_map->characters()->addGameItem(item);
    } else if (item->itemType() == GameItem::TypeEnemy) {
        m_map->enemies()->addGameItem(item);
    } else {
        m_map->items()->addGameItem(item);
    }

}

void GameMapEditor::saveMap()
{
    QString saveGameName(Game::instance()->settings()->mapEditorPath() + QDir::separator() + m_map->name());
    qCDebug(dcMapEditor()) << "Save map to" << saveGameName;
    m_dataManager->saveMap(m_map, saveGameName);
}

void GameMapEditor::deleteAll()
{
    qCDebug(dcMapEditor()) << "Remove all items and objects";
    m_map->objects()->clearModel();
    m_map->items()->clearModel();
    m_map->chests()->clearModel();
    m_map->characters()->clearModel();
    m_map->enemies()->clearModel();
}

void GameMapEditor::onDataManagerStateChanged(DataManager::State state)
{
    qCDebug(dcMapEditor()) << "Datamanager state changed" << state;
}

