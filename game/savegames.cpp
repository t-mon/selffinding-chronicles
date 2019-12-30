#include "savegames.h"

SaveGames::SaveGames(QObject *parent) : QAbstractListModel(parent)
{

}

QList<SaveGame *> SaveGames::saveGames() const
{
    return m_saveGames;
}

SaveGame *SaveGames::get(int index)
{
    if (index >= m_saveGames.count() || index < 0)
        return nullptr;

    return m_saveGames.at(index);
}

int SaveGames::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_saveGames.count();
}

QVariant SaveGames::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_saveGames.count())
        return QVariant();

    SaveGame *saveGame = m_saveGames.at(index.row());
    if (!saveGame)
        return QVariant();

    if (role == IdRole) {
        return saveGame->id();
    } else if (role == MapNameRole) {
        return saveGame->mapName();
    } else if (role == TimestampRole) {
        return saveGame->dateTime().toSecsSinceEpoch();
    } else if (role == DateTimeStringRole) {
        return saveGame->dateTime().toString("dd.MM.yyyy hh:mm:ss");
    } else if (role == ScreenshotFileNameRole) {
        return saveGame->screenShotFileName();
    }

    return QVariant();
}

void SaveGames::addSaveGame(SaveGame *saveGame)
{
    if (m_saveGames.contains(saveGame))
        return;

    saveGame->setParent(this);

    beginInsertRows(QModelIndex(), m_saveGames.count(), m_saveGames.count());
    m_saveGames.append(saveGame);
    endInsertRows();

    emit countChanged(m_saveGames.count());
}

void SaveGames::removeSaveGame(SaveGame *saveGame)
{
    if (!m_saveGames.contains(saveGame))
        return;

    int index = m_saveGames.indexOf(saveGame);
    beginRemoveRows(QModelIndex(), index, index);
    m_saveGames.removeAt(index);
    endRemoveRows();

    emit countChanged(m_saveGames.count());
}

void SaveGames::clearModel()
{
    beginResetModel();
    foreach (SaveGame *saveGame, m_saveGames) {
        saveGame->deleteLater();
    }
    m_saveGames.clear();
    endResetModel();

    emit countChanged(m_saveGames.count());
}

QHash<int, QByteArray> SaveGames::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[IdRole] = "id";
    roles[MapNameRole] = "mapName";
    roles[TimestampRole] = "timestamp";
    roles[DateTimeStringRole] = "dateTimeString";
    roles[ScreenshotFileNameRole] = "screenshotFileName";
    return roles;
}
