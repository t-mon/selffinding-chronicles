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

    Q_PROPERTY(QSize worldSize READ worldSize NOTIFY worldSizeChanged)
    Q_PROPERTY(QColor worldBackgroundColor READ worldBackgroundColor NOTIFY worldBackgroundColorChanged)

    Q_PROPERTY(Character *player READ player NOTIFY playerChanged)
    Q_PROPERTY(GameObjects *objects READ objects CONSTANT)
    Q_PROPERTY(GameItems *items READ items CONSTANT)
    Q_PROPERTY(GameItems *chests READ chests CONSTANT)
    Q_PROPERTY(GameItems *enemies READ enemies CONSTANT)
    Q_PROPERTY(GameItems *characters READ characters CONSTANT)

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
    QSize worldSize() const;
    QColor worldBackgroundColor() const;

    Character *player() const;
    GameObjects *objects() const;
    GameItems *items() const;
    GameItems *chests() const;
    GameItems *enemies() const;
    GameItems *characters() const;

    void createItem(const QString &resourcePath, const QPointF &position);

private:
    State m_state = StateIdle;
    QMutex m_stateMutex;

    SaveGames *m_saveGames = nullptr;
    SaveGame *m_saveGame = nullptr;
    QMutex m_saveGameMutex;
    QImage m_screenshot;

    QDir m_saveGameDir;
    QString m_saveGameName;
    QMutex m_saveGameNameMutex;

    QSize m_worldSize;
    QColor m_worldBackgroundColor = QColor("#307a78");

    Map *m_map = nullptr;
    QMutex m_mapMutex;

    Character *m_player = nullptr;
    QMutex m_playerMutex;

    GameObjects *m_objects = nullptr;
    GameItems *m_items = nullptr;
    GameItems *m_chests = nullptr;
    GameItems *m_enemies = nullptr;
    GameItems *m_characters = nullptr;

    // Set members
    void setState(State state);
    void setWorldSize(const QSize &worlSize);
    void setWorldBackgroundColor(const QColor &color);
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

    void worldSizeChanged(const QSize &worldSize);
    void worldBackgroundColorChanged(const QColor &backgroundColor);
    void viewSizeChanged(const QSizeF &viewSize);
    void viewOffsetChanged(const QPointF &viewOffset);
    void playerChanged(Character *player);

private slots:
    void onThreadFinished();

public slots:
    void initSaveGames();
    void resetData();

    void startNewGame();
    void saveMap(Map *map, const QString &saveGameName);
    void saveGame(const QImage &screenshot);
    void loadGame(SaveGame *saveGame);

};

#endif // DATAMANAGER_H
