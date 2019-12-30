#ifndef SAVEGAMES_H
#define SAVEGAMES_H

#include <QObject>
#include <QAbstractListModel>

#include "savegame.h"

class SaveGames : public QAbstractListModel
{
    Q_OBJECT
public:
    enum SaveGameRole {
        IdRole,
        TimestampRole,
        DateTimeStringRole,
        MapNameRole,
        ScreenshotFileNameRole
    };
    Q_ENUM(SaveGameRole)

    explicit SaveGames(QObject *parent = nullptr);

    QList<SaveGame *> saveGames() const;
    Q_INVOKABLE SaveGame *get(int index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void addSaveGame(SaveGame *saveGame);
    void removeSaveGame(SaveGame *saveGame);

    void clearModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<SaveGame *> m_saveGames;

signals:
    void countChanged(int count);

};

#endif // SAVEGAMES_H
