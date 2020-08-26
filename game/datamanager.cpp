
#include "game.h"
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
    m_saveGames = new SaveGames(this);

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

QString DataManager::saveGameName() const
{
    return m_saveGameName;
}

SaveGames *DataManager::saveGames() const
{
    return m_saveGames;
}

Map *DataManager::map() const
{
    return m_map;
}

Character *DataManager::player() const
{
    return m_player;
}

void DataManager::createItem(const QString &resourcePath, const QPointF &position)
{
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

void DataManager::setState(DataManager::State state)
{
    if (m_state == state)
        return;

    qCDebug(dcDataManager()) << "State changed" << state;
    m_stateMutex.lock();
    m_state = state;
    emit stateChanged(m_state);
    m_stateMutex.unlock();

    // Check if we don't have to start the thread
    if (state == StateIdle)
        return;

    // Start the thread
    start();
}

void DataManager::setSaveGameName(const QString &saveGameName)
{
    if (m_saveGameName == saveGameName)
        return;

    m_saveGameName = saveGameName;
    emit saveGameNameChanged(m_saveGameName);
}

void DataManager::startNewGameTask()
{
    qCDebug(dcDataManager()) << "Start loading new game";

    // Create map
    QMutexLocker mapLocker(&m_mapMutex);
    m_map = new Map(nullptr);
    m_map->loadMap(":/maps/test-character.json");
    //m_map->loadMap(":/maps/test-door.json");
    //m_map->loadMap(":/maps/test-environment.json");
    //m_map->loadMap(":/maps/test-chest.json");
    //m_map->loadMap(":/maps/test-monster.json");

    // Create default player
    qCDebug(dcDataManager()) << "Create default player";
    QMutexLocker playerLocker(&m_playerMutex);
    GameItem *playerGameItem = DataLoader::loadGameItemFromResourcePath(":/gamedata/characters/player.json", m_map);
    m_player = qobject_cast<Character *>(playerGameItem);
    m_player->setPosition(m_map->playerStartPosition());
    m_player->setIsPlayer(true);

    // Add player to characters
    m_map->setPlayer(m_player);

    // Push the map back to the main thread
    qCDebug(dcDataManager()) << "Push loaded map back to main thread" << QCoreApplication::instance()->thread();
    m_map->moveToThread(QCoreApplication::instance()->thread());
}

void DataManager::startNewGameTaskFinished()
{
    qCDebug(dcDataManager()) << "Loading thread for new game finished.";
    loadGameTaskFinished();
}

void DataManager::loadGameTask()
{
    qCDebug(dcDataManager()) << "Start loading" << m_saveGame;
    QMutexLocker mapLocker(&m_mapMutex);
    m_map = new Map(nullptr);
    m_map->loadMap(m_saveGame->mapFileName());

    // Load player
    QVariantMap generalMap = DataLoader::loadJsonData(m_saveGame->generalFileName());
    QVariantMap playerMap = generalMap.value("player").toMap();
    QPointF position = QPointF(playerMap.value("x").toDouble(), playerMap.value("y").toDouble());
    m_player = DataLoader::createCharacterObject(playerMap.value("data").toString(), playerMap, position, m_map);
    m_player->setIsPlayer(true);
    m_player->setName(Game::instance()->settings()->playerName());

    // Add player to characters
    m_map->setPlayer(m_player);


    // Push the map back to the main thread
    qCDebug(dcDataManager()) << "Push loaded map back to main thread" << QCoreApplication::instance()->thread();
    m_map->moveToThread(QCoreApplication::instance()->thread());
}

void DataManager::loadGameTaskFinished()
{
    qCDebug(dcDataManager()) << "Loading finished.";
    qCDebug(dcDataManager()) << "Initialize the map data" << m_map->thread();
    m_map->setParent(this);
    qCDebug(dcDataManager()) << m_map;
}

void DataManager::saveGameTask()
{
    qCDebug(dcDataManager()) << "Start saving game" << QThread::currentThreadId();

    // Check we have currelty a savegame
    QMutexLocker saveGameLocker(&m_saveGameMutex);
    if (!m_saveGame) {
        m_saveGame = new SaveGame(nullptr);
        m_saveGame->setId(QUuid::createUuid().toString().remove('-').remove('{').remove('}'));
        m_saveGames->addSaveGame(m_saveGame);
        qCDebug(dcDataManager()) << "Created new savegame with ID" << m_saveGame->id();
    }

    if (!m_saveGame->saveGameDirectory().exists()) {
        if (!QDir().mkpath(m_saveGame->saveGameDirectory().absolutePath())) {
            qCWarning(dcDataManager()) << "Could not create save game directory" << m_saveGame->saveGameDirectory().absolutePath();
            return;
        }

        qCDebug(dcDataManager()) << "Created successfully save game dir" << m_saveGame->saveGameDirectory().absolutePath();
    }

    m_saveGame->setDateTime(QDateTime::currentDateTime());
    m_saveGame->setMapName(m_map->name());
    m_saveGame->setCurrentMapFileName(m_map->resourceFileName());

    // general
    QVariantMap generalMap;
    generalMap.insert("name", m_saveGameName);
    generalMap.insert("timestamp", m_saveGame->dateTime().toSecsSinceEpoch());
    generalMap.insert("mapName", m_saveGame->mapName());
    generalMap.insert("currentMapFileName", m_saveGame->currentMapFileName());

    // TODO: Game statistics...

    // Save the player
    generalMap.insert("player", DataLoader::characterToVariantMap(m_map->player()));

    // TODO: save player story progress

    // Map file
    QVariantMap map;
    map.insert("name", m_map->name());
    map.insert("resourcePath", m_map->resourcePath());
    map.insert("width", m_map->size().width());
    map.insert("height", m_map->size().height());
    map.insert("playerStartX", m_map->playerStartPosition().x());
    map.insert("playerStartY", m_map->playerStartPosition().y());
    map.insert("backgroundColor", m_map->backgroundColor().name());
    map.insert("objects", DataLoader::gameObjectsToVariantList(m_map->objects()));
    map.insert("items", DataLoader::gameItemsToVariantList(m_map->items()));
    map.insert("chests", DataLoader::chestsToVariantList(m_map->chests()));
    map.insert("enemies", DataLoader::enemiesToVariantList(m_map->enemies()));
    // Note: the player will not be saved in DataLoader::charactersToVariantList(m_characters)
    map.insert("characters", DataLoader::charactersToVariantList(m_map->characters()));

    // Write the current map
    QFile saveGameMapFile(m_saveGame->saveGameDirectory().absolutePath() + QDir::separator() + m_map->resourceFileName());
    if (!saveGameMapFile.open(QFile::WriteOnly)) {
        qCWarning(dcDataManager()) << "Could not open map file for saving map data" << saveGameMapFile.errorString();
        return;
    }

    saveGameMapFile.write(QJsonDocument::fromVariant(map).toJson(QJsonDocument::Indented));
    saveGameMapFile.close();
    qCDebug(dcDataManager()) << "Saved map successfully" << saveGameMapFile.fileName();

    // Write the general data
    QFile saveGameFile(m_saveGame->saveGameDirectory().absolutePath() + QDir::separator() + "general.json");
    if (!saveGameFile.open(QFile::WriteOnly)) {
        qCWarning(dcDataManager()) << "Could not open general file for saving general data" << saveGameFile.errorString();
        return;
    }

    saveGameFile.write(QJsonDocument::fromVariant(generalMap).toJson(QJsonDocument::Indented));

    if (!m_screenshot.isNull()) {
        if (!m_screenshot.save(m_saveGame->screenShotFileName(), "png")) {
            qCWarning(dcDataManager()) << "Could not save screenshot image to" << m_saveGame->screenShotFileName();
        } else {
            qCDebug(dcDataManager()) << "Saved successfully screenshot image";
        }
    }

    qCDebug(dcDataManager()) << "Saved general data successfully" << m_saveGame;
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
    map.insert("objects", DataLoader::gameObjectsToVariantList(m_map->objects()));
    map.insert("items", DataLoader::gameItemsToVariantList(m_map->items()));
    map.insert("chests", DataLoader::gameItemsToVariantList(m_map->chests()));
    map.insert("characters", DataLoader::charactersToVariantList(m_map->characters()));
    map.insert("enemies", DataLoader::enemiesToVariantList(m_map->enemies()));

    QDir storageDir(QFileInfo(m_saveGameName).absolutePath());
    qCDebug(dcDataManager()) << "Verify storage dir" << storageDir.absolutePath();
    if (!storageDir.exists()) {
        qCDebug(dcDataManager()) << "Creating directory" << storageDir.absolutePath();
        if (!storageDir.mkpath(storageDir.absolutePath())) {
            qCWarning(dcDataManager()) << "Could not create path for storing map" << storageDir.absolutePath();
        }
    }

    QFile saveGameFile(m_saveGameName);
    if (!saveGameFile.open(QFile::WriteOnly)) {
        qCWarning(dcDataManager()) << "Could not open file for saving" << m_saveGameName << saveGameFile.errorString();
        return;
    }

    saveGameFile.write(QJsonDocument::fromVariant(map).toJson(QJsonDocument::Indented));
    saveGameFile.close();
}

void DataManager::saveMapTaskFinished()
{
    qCDebug(dcDataManager()) << "Save map task finsihed.";
    QMutexLocker saveGameLocker(&m_saveGameNameMutex);

    if (m_saveGame) {
        m_saveGame->setParent(this);
    }
}

void DataManager::run()
{
    // Init
    qCDebug(dcDataManager()) << "Start thread" << QThread::currentThreadId();
    QElapsedTimer timer;
    QMutexLocker saveGameLocker(&m_saveGameNameMutex);

    // Get current state
    m_stateMutex.lock();
    State state = m_state;
    m_stateMutex.unlock();

    // Start measurement timer
    timer.restart();

    // Load save data
    switch (state) {
    case StateLoading:
        loadGameTask();
        qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        break;
    case StateStarting:
        startNewGameTask();
        qCDebug(dcDataManager()) << "Loading finished" << timer.elapsed() << "[ms]";
        break;
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
        qCWarning(dcDataManager()) << "Could not reset data, the thread is still runing.";
        return;
    }

    if (m_map) {
        m_map->deleteLater();
        m_map = nullptr;
    }

    if (m_player) {
        m_player = nullptr;
    }


    setSaveGameName(QString());
    setState(StateIdle);
}

void DataManager::initSaveGames()
{
    qCDebug(dcDataManager()) << "Initialize savegames" << Game::instance()->settings()->saveGamePath();
    QDir saveGamesDir(Game::instance()->settings()->saveGamePath());
    QStringList saveGamesList = saveGamesDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Readable);
    foreach (const QString &id, saveGamesList) {
        qCDebug(dcDataManager()) << "Start loading save game" << id;
        SaveGame *saveGame = new SaveGame(this);
        saveGame->setId(id);

        QVariantMap saveGameMap = DataLoader::loadJsonData(saveGame->saveGameDirectory().absolutePath() + QDir::separator() + "general.json");
        saveGame->setMapName(saveGameMap.value("mapName").toString());
        saveGame->setCurrentMapFileName(saveGameMap.value("currentMapFileName").toString());
        saveGame->setDateTime(QDateTime::fromSecsSinceEpoch(saveGameMap.value("timestamp").toUInt()));
        m_saveGames->addSaveGame(saveGame);
    }
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

void DataManager::saveMap(Map *map, const QString &saveGameName)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not save map to" << saveGameName << ", the thread is still runnging.";
        return;
    }

    QMutexLocker saveGameLocker(&m_saveGameNameMutex);
    qCDebug(dcDataManager()) << "Save the map to" << saveGameName;
    m_saveGameName = saveGameName;

    QMutexLocker mapLocker(&m_mapMutex);
    m_map = map;

    setState(StateSavingMap);
}

void DataManager::saveGame(const QImage &screenshot)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not save game" << screenshot << ", the thread is still runnging.";
        return;
    }

    m_screenshot = screenshot;
    setState(StateSaving);
}

void DataManager::loadGame(SaveGame *saveGame)
{
    if (isRunning()) {
        qCWarning(dcDataManager()) << "Could not load savegame" << saveGame << ", the thread is still runnging.";
        return;
    }

    qCDebug(dcDataManager()) << "Start loading savegame" << saveGame;

    QMutexLocker saveGameLocker(&m_saveGameNameMutex);
    m_saveGame = saveGame;

    // Reset all data
    resetData();
    setState(StateLoading);
}
