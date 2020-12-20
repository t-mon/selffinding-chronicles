#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QDir>
#include <QSizeF>
#include <QImage>
#include <QThread>
#include <QObject>
#include <QPointF>
#include <QMutexLocker>

#include "map.h"
#include "savegames.h"

class DataManager : public QThread
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(SaveGames *saveGames READ saveGames CONSTANT)
    Q_PROPERTY(QString saveGameName READ saveGameName NOTIFY saveGameNameChanged)
    Q_PROPERTY(Map *map READ map NOTIFY mapChanged)

public:
    enum State {
        StateIdle,
        StateStarting,
        StateLoading,
        StateSaving,
        StateSavingMap
    };
    Q_ENUM(State)

    explicit DataManager(QObject *parent = nullptr);
    ~DataManager() override;

    State state() const;
    QString saveGameName() const;
    SaveGames *saveGames() const;

    Map *map() const;

    void createItem(const QString &resourcePath, const QPointF &position);

private:
    State m_state = StateIdle;
    mutable QMutex m_stateMutex;

    SaveGames *m_saveGames = nullptr;
    SaveGame *m_saveGame = nullptr;
    QImage m_screenshot;
    mutable QMutex m_saveGameMutex;

    QDir m_saveGameDir;
    QString m_saveGameName;
    mutable QMutex m_saveGameNameMutex;

    Map *m_map = nullptr;
    mutable QMutex m_mapMutex;
    QString m_newGameMapPath;

    Character *createDefaultPlayer();

    // Set members
    void setState(State state);
    void setSaveGameName(const QString &saveGameName);

    // Thread tasks
    void startNewGameTask();
    void startNewGameTaskFinished();

    void loadGameTask();
    void loadGameTaskFinished();

    void saveGameTask();
    void saveGameTaskFinished();

    void saveMapTask();
    void saveMapTaskFinished();

protected:
    void run() override;

signals:
    void stateChanged(State state);
    void saveGameNameChanged(const QString &saveGameName);

    void loadingFinished();
    void savingFinished();

    void mapChanged(Map *map);

private slots:
    void onThreadFinished();

public slots:
    void initSaveGames();
    void resetData();

    void startNewGame();
    void startTutorial(const QString &tutorialMap);

    void saveMap(Map *map, const QString &saveGameName);
    void saveGame(const QImage &screenshot);
    void loadGame(SaveGame *saveGame);

};

#endif // DATAMANAGER_H
