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
    connect(m_activeObjects, &GameObjectsProxy::gameObjectActiveChanged, this, [](GameObject *object, bool active){
        if (active) {
            qCDebug(dcMapEditorData()) << "[+] Object changed to active" << object;
        } else {
            qCDebug(dcMapEditorData()) << "[-] Object changed to inactive" << object;
        }

        object->setActive(active);
    });


    m_activeItems = new GameItemsProxy(this);
    m_activeItems->setGameItems(m_dataManager->items());
    connect(m_activeItems, &GameItemsProxy::gameItemActiveChanged, this, [](GameItem *item, bool active){
        if (active) {
            qCDebug(dcMapEditorData()) << "[+] Item changed to active" << item;
        } else {
            qCDebug(dcMapEditorData()) << "[-] Item changed to inactive" << item;
        }

        item->setActive(active);
    });


    m_activeEnemies = new GameItemsProxy(this);
    m_activeEnemies->setGameItems(m_dataManager->enemies());
    connect(m_activeEnemies, &GameItemsProxy::gameItemActiveChanged, this, [](GameItem *item, bool active){
        if (active) {
            qCDebug(dcMapEditorData()) << "[+] Enemie changed to active" << item;
        } else {
            qCDebug(dcMapEditorData()) << "[-] Enemie changed to inactive" << item;
        }

        item->setActive(active);
    });

    m_activeCharacters = new GameItemsProxy(this);
    m_activeCharacters->setGameItems(m_dataManager->characters());
    connect(m_activeCharacters, &GameItemsProxy::gameItemActiveChanged, this, [](GameItem *item, bool active){
        if (active) {
            qCDebug(dcMapEditorData()) << "[+] Character changed to active" << item;
        } else {
            qCDebug(dcMapEditorData()) << "[-] Character changed to inactive" << item;
        }

        item->setActive(active);
    });

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

QRectF GameMapEditor::viewWindow() const
{
    return m_viewWindow;
}

void GameMapEditor::setViewWindow(const QRectF &viewWindow)
{
    if (m_viewWindow == viewWindow)
        return;

    //qCDebug(dcMapEditor()) << "Editor view window changed" << viewWindow;
    m_viewWindow = viewWindow;
    emit viewWindowChanged(m_viewWindow);

    m_activeObjects->setViewFilter(m_viewWindow);
    m_activeItems->setViewFilter(m_viewWindow);
    //m_activeChests->setViewFilter(m_viewWindow);
    m_activeCharacters->setViewFilter(m_viewWindow);
    m_activeEnemies->setViewFilter(m_viewWindow);
    //m_weatherAreaProxy->setViewFilter(m_viewWindow);
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

void GameMapEditor::deleteAll()
{
    qCDebug(dcMapEditor()) << "Remove all items and objects";
    m_dataManager->resetData();
}

void GameMapEditor::onDataManagerStateChanged(DataManager::State state)
{
    qCDebug(dcMapEditor()) << "Datamanager state changed" << state;
}

