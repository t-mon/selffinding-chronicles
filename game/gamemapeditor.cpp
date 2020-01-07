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

    m_activeObjects = new GameObjectsProxy(this);
    m_activeObjects->setGameObjects(m_dataManager->objects());

    m_activeItems = new GameItemsProxy(this);
    m_activeItems->setGameItems(m_dataManager->items());

    m_activeEnemies = new GameItemsProxy(this);
    m_activeEnemies->setGameItems(m_dataManager->enemies());

    m_activeCharacters = new GameItemsProxy(this);
    m_activeCharacters->setGameItems(m_dataManager->characters());

//    connect(m_activeItems, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);
//    connect(m_activeEnemies, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);
//    connect(m_activeCharacters, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);

//    loadAvailableObjects();
//    loadAvailableGameItems();

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

DataManager *GameMapEditor::dataManager() const
{
    return m_dataManager;
}

GameItemsProxy *GameMapEditor::activeItems() const
{
    return m_activeItems;
}

GameItemsProxy *GameMapEditor::activeCharacters() const
{
    return m_activeCharacters;
}

GameItemsProxy *GameMapEditor::activeEnemies() const
{
    return m_activeEnemies;
}

Map *GameMapEditor::map() const
{
    return m_map;
}

void GameMapEditor::createNewMap()
{
    qCDebug(dcMapEditor()) << "Create new map.";
    if (m_map) {
        qCWarning(dcMapEditor()) << "Deleting current loaded map. FIXE: check if autosave";
        m_map->deleteLater();
        m_map = nullptr;
    }

    m_dataManager->resetData();
    m_map = new Map(m_dataManager->objects(), m_dataManager->items(), m_dataManager->enemies(), m_dataManager->characters());
}

void GameMapEditor::placeItemOnMap(const QString &resourcePath, const QPointF &position)
{
    qCDebug(dcMapEditor()) << "Place item" << resourcePath << position;
    m_dataManager->createItem(resourcePath, position);
}

void GameMapEditor::saveMap()
{
    QString saveGameName(Game::instance()->settings()->mapEditorPath() + QDir::separator() + m_map->name());
    qCDebug(dcMapEditor()) << "Save map to" << saveGameName;
    m_dataManager->saveMap(m_map, saveGameName);
}

void GameMapEditor::onDataManagerStateChanged(DataManager::State state)
{
    qCDebug(dcMapEditor()) << "Datamanager state changed" << state;
}

void GameMapEditor::onEditorViewOffsetChanged(const QPointF &offset)
{
    m_editorViewOffset = offset;

    // TODO: evaluate active items
}

void GameMapEditor::onEditorViewSizeChanged(const QSizeF &size)
{
    m_editorViewSize = size;

    // TODO: evaluate active items

}
