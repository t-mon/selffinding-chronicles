#include "gamemapeditor.h"
#include "debugcategories.h"
#include "dataloader.h"
#include "game.h"

GameMapEditor::GameMapEditor(QObject *parent) :
    QObject(parent)
{
    m_availableItems = new GameItems(this);

    m_dataManager = new DataManager(this);
    connect(m_dataManager, &DataManager::stateChanged, this, &GameMapEditor::onDataManagerStateChanged);

    m_activeItems = new GameItemsProxy(this);
    m_activeItems->setGameItems(m_dataManager->items());

    m_activeEnemies = new GameItemsProxy(this);
    m_activeEnemies->setGameItems(m_dataManager->enemies());

    m_activeCharacters = new GameItemsProxy(this);
    m_activeCharacters->setGameItems(m_dataManager->characters());

//    connect(m_activeItems, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);
//    connect(m_activeEnemies, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);
//    connect(m_activeCharacters, &GameItemsProxy::gameItemActiveChanged, this, &Engine::onGameItemActiveChanged);

}

GameItems *GameMapEditor::availableItems() const
{
    return m_availableItems;
}

void GameMapEditor::loadAvailableGameItems()
{
    if (!m_availableItems->gameItems().isEmpty()) {
        qCDebug(dcMapEditor()) << "Available items already loaded. Doing nothing";
        return;
    }

    QString gameDataPath = ":/gamedata/";
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
    m_map = new Map(m_dataManager->items(), m_dataManager->enemies(), m_dataManager->characters());

}

void GameMapEditor::placeItemOnMap(const QString &resourcePath, const QPointF &position)
{
    qCDebug(dcMapEditor()) << "Place item" << resourcePath << position;
    m_dataManager->createItem(resourcePath, position);
}

void GameMapEditor::saveMap()
{
    QString saveGameFileName(Game::instance()->settings()->settingsPath() + "/mapeditor/" + m_map->name());
    qCDebug(dcMapEditor()) << "Save map to" << saveGameFileName;
    m_dataManager->saveMap(m_map, saveGameFileName);
}

void GameMapEditor::onDataManagerStateChanged(DataManager::State state)
{
    qCDebug(dcMapEditor()) << "Dataloader state changed" << state;
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
