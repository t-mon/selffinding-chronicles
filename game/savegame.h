#ifndef SAVEGAME_H
#define SAVEGAME_H

#include <QDir>
#include <QObject>
#include <QDateTime>

class SaveGame : public QObject
{
    Q_OBJECT
public:
    explicit SaveGame(QObject *parent = nullptr);

    QString id() const;
    void setId(const QString &id);

    QDir saveGameDirectory() const;

    QDateTime dateTime() const;
    void setDateTime(const QDateTime &dateTime);

    QString mapName() const;
    void setMapName(const QString &mapName);

    QString currentMapFileName() const;
    void setCurrentMapFileName(const QString &currentMapFileName);

    QString screenShotFileName() const;
    QString mapFileName() const;
    QString generalFileName() const;

private:
    QString m_id;
    QDir m_saveGameDirectory;
    QDateTime m_dateTime;
    QString m_mapName;
    QString m_currentMapFileName;

};

QDebug operator<<(QDebug debug, SaveGame *saveGame);

#endif // SAVEGAME_H
