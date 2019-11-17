#include "dataloader.h"
#include "datamanager.h"
#include "debugcategories.h"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDateTime>
#include <QElapsedTimer>
#include <QJsonDocument>
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
    if (state == StateIdle)
        return;

    // Start the thread
    start();
}

void DataManager::setSaveGameName(const QString &saveGameFileName)
{
    if (m_saveGameFileName == saveGameFileName)
        return;

    m_saveGameFileName = saveGameFileName;
    emit saveGameFileNameChanged(m_saveGameFileName);

    // Create json and dump data
}

void DataManager::startNewGameTask()
{
    qCDebug(dcDataManager()) << "Start loading new game";

    // Create map
    QMutexLocker mapLocker(&m_mapMutex);
    m_map = new Map(nullptr);
    m_map->loadMap(":/maps/test-character.json");
    //m_map->loadMap(":/maps/test-environment.json");
    //m_map->loadMap(":/maps/test-monster.json");

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
}

void DataManager::startNewGameTaskFinished()
{
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
}

void DataManager::loadGameTask()
{
    qCDebug(dcDataManager()) << "Start loading" << m_saveGameFileName;
    QMutexLocker mapLocker(&m_mapMutex);
    m_map = new Map(nullptr);
    m_map->loadMap(m_saveGameFileName);
    m_map->moveToThread(QCoreApplication::instance()->thread());

    // Load map

    // Load statistics

    // Load player
}

void DataManager::loadGameTaskFinished()
{
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

}

void DataManager::saveGameTask()
{
    qCDebug(dcDataManager()) << "Start saving game" << QThread::currentThreadId();

    QVariantMap saveGame;
    saveGame.insert("name", m_saveGameFileName);
    saveGame.insert("timestamp", QDateTime::currentDateTime().toSecsSinceEpoch());

    // Game statistics

    // TODO: current map

    QVariantList maps;
    QVariantMap map;
    map.insert("width", m_map->size().width());
    map.insert("height", m_map->size().height());
    map.insert("playerStartX", m_map->playerStartPosition().x());
    map.insert("playerStartY", m_map->playerStartPosition().y());
    map.insert("backgroundColor", m_map->backgroundColor().name());



    // Items
//    saveGame.insert("items", DataLoader::gameItemsToVariantList(m_items));

    // Enemies
    //saveGame.insert("enemies", )

    // Characters


    maps.append(map);
    saveGame.insert("maps", maps);

    QFile saveGameFile(m_saveGameFileName);
    if (!saveGameFile.open(QFile::WriteOnly)) {
        qCWarning(dcDataManager()) << "Could not open file for saving" << m_saveGameFileName;
        return;
    }

    saveGameFile.write(QJsonDocument::fromVariant(saveGame).toJson(QJsonDocument::Indented));
}

void DataManager::saveGameTaskFinished()
{
    qCDebug(dcDataManager()) << "Save game thread finished.";
}

void DataManager::saveMapTask()
{
    qCDebug(dcDataManager()) << "Start saving map" << QThread::currentThreadId();

    QMutexLocker mapLocker(&m_mapMutex);

    QVariantMap map;
    map.insert("name", m_map->name());
    map.insert("width", m_map->size().width());
    map.insert("height", m_map->size().height());
    map.insert("playerStartX", m_map->playerStartPosition().x());
    map.insert("playerStartY", m_map->playerStartPosition().y());
    map.insert("backgroundColor", m_map->backgroundColor().name());
    map.insert("items", DataLoader::gameItemsToVariantList(m_map->items()));
    map.insert("characters", DataLoader::charactersToVariantList(m_map->characters()));

    QVariantList enemies;

    QDir storageDir(QFileInfo(m_saveGameFileName).absolutePath());
    qCDebug(dcDataManager()) << "Verify storage dir" << storageDir.absolutePath();
    if (!storageDir.exists()) {
        qCDebug(dcDataManager()) << "Creating directory" << storageDir.absolutePath();
        if (!storageDir.mkpath(storageDir.absolutePath())) {
            qCWarning(dcDataManager()) << "Could not create path for storing map" << storageDir.absolutePath();
        }
    }

    QFile saveGameFile(m_saveGameFileName);
    if (!saveGameFile.open(QFile::WriteOnly)) {
        qCWarning(dcDataManager()) << "Could not open file for saving" << m_saveGameFileName << saveGameFile.errorString();
        return;
    }

    saveGameFile.write(QJsonDocument::fromVariant(map).toJson(QJsonDocument::Indented));
}

void DataManager::saveMapTaskFinished()
{
    qCDebug(dcDataManager()) << "Save map task finsihed.";
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
        loadGameTask();
        qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        break;
    }
    case StateStarting: {
        startNewGameTask();
        qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        break;
    }
    case StateSaving:
        saveGameTask();
        qCDebug(dcDataManager()) << "Saving finished" << timer.elapsed() << "[ms]";
        break;
    case StateSavingMap:
        saveMapTask();
        qCDebug(dcDataManager()) << "Saving map finished" << timer.elapsed() << "[ms]";
        break;
    default:
        break;
    }
}

void DataManager::onThreadFinished()
{
    switch (m_state) {
    case StateStarting:
        startNewGameTaskFinished();
        break;
    case StateLoading:
        loadGameTaskFinished();
        break;
    case StateSaving:
        saveGameTaskFinished();
        break;
    case StateSavingMap:
        saveMapTaskFinished();
        break;
    default:
        qCDebug(dcDataManager()) << "Thread finished. Unhandled state.";
        break;
    }

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

void DataManager::saveMap(Map *map, const QString &saveGameFileName)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not save map to" << saveGameFileName << ", the thread is still runnging.";
        return;
    }

    QMutexLocker saveGameLocker(&m_saveGameFileNameMutex);
    qCDebug(dcDataManager()) << "Save the map to" << saveGameFileName;
    m_saveGameFileName = saveGameFileName;

    QMutexLocker mapLocker(&m_mapMutex);
    m_map = map;

    setState(StateSavingMap);
}

void DataManager::saveGame(const QString &saveGameFileName)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not save game" << saveGameFileName << ", the thread is still runnging.";
        return;
    }

    QMutexLocker saveGameLocker(&m_saveGameFileNameMutex);
    qCDebug(dcDataManager()) << "Save the game to" << saveGameFileName;
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
