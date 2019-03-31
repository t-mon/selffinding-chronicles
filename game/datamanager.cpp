#include "dataloader.h"
#include "datamanager.h"
#include "debugcategories.h"

#include <QDateTime>
#include <QElapsedTimer>
#include <QCoreApplication>

DataManager::DataManager(QObject *parent) :
    QThread(parent)
{
    m_items = new GameItems(this);
    m_enemies = new GameItems(this);
    m_characters = new GameItems(this);

    connect(this, &DataManager::finished, this, &DataManager::onThreadFinished);
}

DataManager::~DataManager()
{
    if (isRunning()) {
        terminate();
        wait();
    }
}

DataManager::State DataManager::state() const
{
    return m_state;
}

QString DataManager::saveGameFileName() const
{
    return m_saveGameFileName;
}

QSize DataManager::worldSize() const
{
    return m_worldSize;
}

QColor DataManager::worldBackgroundColor() const
{
    return m_worldBackgroundColor;
}

void DataManager::setWorldSize(const QSize &worldSize)
{
    if (m_worldSize == worldSize)
        return;

    m_worldSize = worldSize;
    emit worldSizeChanged(m_worldSize);
}

void DataManager::setWorldBackgroundColor(const QColor &color)
{
    if (m_worldBackgroundColor == color)
        return;

    m_worldBackgroundColor = color;
    emit worldBackgroundColorChanged(m_worldBackgroundColor);
}

Character *DataManager::player() const
{
    return m_player;
}

GameItems *DataManager::items() const
{
    return m_items;
}

GameItems *DataManager::enemies() const
{
    return m_enemies;
}

GameItems *DataManager::characters() const
{
    return m_characters;
}

void DataManager::createItem(const QString &resourcePath, const QPointF &position)
{
    GameItem *item = DataLoader::loadGameItemFromResourcePath(resourcePath, this);
    item->setPosition(position);

    if (item->itemType() == GameItem::TypeCharacter) {
        m_characters->addGameItem(item);
    } else if (item->itemType() == GameItem::TypeEnemy) {
        m_enemies->addGameItem(item);
    } else {
        m_items->addGameItem(item);
    }
}

void DataManager::setState(DataManager::State state)
{
    if (m_state == state)
        return;

    qCDebug(dcDataManager()) << "State changed" << state;
    m_stateMutex.lock();
    m_state = state;
    emit stateChanged(m_state);
    m_stateMutex.unlock();

    // Start the thread
    if (state != StateIdle) {

        //        qCDebug(dcDataManager()) << "Start loading new game";

        //        // Create map
        //        m_map = new Map(this);
        //        qCDebug(dcDataManager()) << "Create map" << m_map->thread();
        //        m_map->loadMap(":/maps/test-environment.json");

        //        // Create player
        //        qCDebug(dcDataManager()) << "Create default player";
        //        GameItem *playerGameItem = DataLoader::loadGameItemFromResourcePath(":/gamedata/characters/player.json", m_map);
        //        m_player = qobject_cast<Character *>(playerGameItem);
        //        m_player->setPosition(m_map->playerStartPosition());
        //        m_player->setIsPlayer(true);

        //        // Add player to characters and move to application thread
        //        m_map->characters()->addGameItem(m_player);

        //        setWorldSize(m_map->size());

        //        // Push the map back to the main thread
        //        qCDebug(dcDataManager()) << "Push loaded map back to main thread" << QCoreApplication::instance()->thread();
        //        //m_map->moveToThread(QCoreApplication::instance()->thread());

        //        //qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        //        onThreadFinished();

        // FIXME
        start();
    }

}

void DataManager::setSaveGameName(const QString &saveGameFileName)
{
    if (m_saveGameFileName == saveGameFileName)
        return;

    m_saveGameFileName = saveGameFileName;
    emit saveGameFileNameChanged(m_saveGameFileName);

    // Create json and dump data
}

void DataManager::loadSaveGame()
{
    // Load map

    // Load statistics

    // Load player

}

void DataManager::saveSaveGame()
{
    QMutexLocker saveGameLocker(&m_saveGameFileNameMutex);
    QVariantMap saveGame;
    saveGame.insert("name", m_saveGameFileName);
    saveGame.insert("timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());

    // TODO
}

void DataManager::run()
{
    // Init
    qCDebug(dcDataManager()) << "Start thread" << QThread::currentThreadId();
    QElapsedTimer timer;
    QMutexLocker saveGameLocker(&m_saveGameFileNameMutex);

    // Get current state
    m_stateMutex.lock();
    State state = m_state;
    m_stateMutex.unlock();

    // Start measurement timer
    timer.restart();

    // Load save data
    switch (state) {
    case StateLoading: {
        qCDebug(dcDataManager()) << "Start loading" << m_saveGameFileName;
        QMutexLocker mapLocker(&m_mapMutex);
        m_map = new Map();
        m_map->loadMap(m_saveGameFileName);
        m_map->moveToThread(QCoreApplication::instance()->thread());
        qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        break;
    }
    case StateStarting: {
        qCDebug(dcDataManager()) << "Start loading new game";

        // Create map
        m_map = new Map(nullptr);
        m_map->loadMap(":/maps/test-environment.json");

        // Create player
        qCDebug(dcDataManager()) << "Create default player";
        GameItem *playerGameItem = DataLoader::loadGameItemFromResourcePath(":/gamedata/characters/player.json", m_map);
        m_player = qobject_cast<Character *>(playerGameItem);
        m_player->setPosition(m_map->playerStartPosition());
        m_player->setIsPlayer(true);

        // Add player to characters and move to application thread
        m_map->characters()->addGameItem(m_player);

        // Set world properties
        setWorldSize(m_map->size());
        setWorldBackgroundColor(m_map->backgroundColor());

        // Push the map back to the main thread
        qCDebug(dcDataManager()) << "Push loaded map back to main thread" << QCoreApplication::instance()->thread();
        m_map->moveToThread(QCoreApplication::instance()->thread());

        qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        break;
    }
    case StateSaving:
        qCDebug(dcDataManager()) << "Start saving" << m_saveGameFileName;
        saveSaveGame();
        qCDebug(dcDataManager()) << "Saving finished" << timer.elapsed() << "[ms]";
        break;
    default:
        break;
    }
}

void DataManager::onThreadFinished()
{
    switch (m_state) {
    case StateStarting: {
        qCDebug(dcDataManager()) << "Loading thread for new game finished.";
        qCDebug(dcDataManager()) << "Initialize the map data" << m_map->thread();
        m_map->setParent(this);
        setWorldSize(m_map->size());

        foreach (GameItem *item, m_map->items()->gameItems()) {
            m_items->addGameItem(item);
        }

        foreach (GameItem *item, m_map->characters()->gameItems()) {
            m_characters->addGameItem(item);
        }

        foreach (GameItem *item, m_map->enemies()->gameItems()) {
            m_enemies->addGameItem(item);
        }

        qCDebug(dcDataManager()) << m_map;

        break;
    }
    case StateLoading: {
        qCDebug(dcDataManager()) << "Loading thread finished";
        qCDebug(dcDataManager()) << "Initialize the map data";
        m_map->setParent(this);
        setWorldSize(m_map->size());

        foreach (GameItem *item, m_map->items()->gameItems()) {
            m_items->addGameItem(item);
        }

        foreach (GameItem *item, m_map->characters()->gameItems()) {
            m_characters->addGameItem(item);
        }

        foreach (GameItem *item, m_map->enemies()->gameItems()) {
            m_enemies->addGameItem(item);
        }

        break;
    }
    default:
        qCDebug(dcDataManager()) << "Thread finished. Unhandled state.";
        break;
    }

    // Emit done
    setState(StateIdle);
}

void DataManager::resetData()
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not reset data, the thread is still runnging.";
        return;
    }

    if (m_map) {
        m_map->deleteLater();
        m_map = nullptr;
    }

    if (m_player) {
        m_player = nullptr;
    }

    m_items->clearModel();
    m_characters->clearModel();
    m_enemies->clearModel();

    setWorldSize(QSize());
    setSaveGameName(QString());
    setState(StateIdle);
}

void DataManager::startNewGame()
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not start a new game, the thread is still runnging.";
        return;
    }

    qCDebug(dcDataManager()) << "Start a new game";

    // Reset all data
    resetData();

    setState(StateStarting);
}

void DataManager::saveGame(const QString &saveGameFileName)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not save game" << saveGameFileName << ", the thread is still runnging.";
        return;
    }

    QMutexLocker saveGameLocker(&m_saveGameFileNameMutex);
    m_saveGameFileName = saveGameFileName;

    setState(StateSaving);
}

void DataManager::loadGame(const QString &saveGameFileName)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not load savegame" << saveGameFileName << ", the thread is still runnging.";
        return;
    }

    QMutexLocker saveGameLocker(&m_saveGameFileNameMutex);
    m_saveGameFileName = saveGameFileName;

    // Reset all data
    resetData();

    setState(StateLoading);
}
