#include "savegame.h"
#include "game.h"

SaveGame::SaveGame(QObject *parent) : QObject(parent)
{

}

QString SaveGame::id() const
{
    return m_id;
}

void SaveGame::setId(const QString &id)
{
    m_id = id;
    m_saveGameDirectory = QDir(Game::instance()->settings()->saveGamePath() + QDir::separator() + m_id);
}

QDir SaveGame::saveGameDirectory() const
{
    return m_saveGameDirectory;
}

QDateTime SaveGame::dateTime() const
{
    return m_dateTime;
}

void SaveGame::setDateTime(const QDateTime &dateTime)
{
    m_dateTime = dateTime;
}

QString SaveGame::mapName() const
{
    return m_mapName;
}

void SaveGame::setMapName(const QString &mapName)
{
    m_mapName = mapName;
}

QString SaveGame::currentMapFileName() const
{
    return m_currentMapFileName;
}

void SaveGame::setCurrentMapFileName(const QString &currentMapFileName)
{
    m_currentMapFileName = currentMapFileName;
}

QString SaveGame::screenShotFileName() const
{
    return m_saveGameDirectory.absolutePath() + QDir::separator() + "screenshot.png";
}

QString SaveGame::mapFileName() const
{
    return m_saveGameDirectory.absolutePath() + QDir::separator() + m_currentMapFileName;
}

QString SaveGame::generalFileName() const
{
    return m_saveGameDirectory.absolutePath() + QDir::separator() + "general.json";
}

QDebug operator<<(QDebug debug, SaveGame *saveGame)
{
    debug.nospace() << "SaveGame(";
    debug.nospace() << saveGame->id() << ", ";
    debug.nospace() << saveGame->mapName() << ", ";
    debug.nospace() << saveGame->currentMapFileName() << ", ";
    debug.nospace() << saveGame->dateTime().toString("dd.MM.yyyy HH:mm") << ")";
    return debug.space();
}
