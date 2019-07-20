#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QSizeF>
#include <QThread>
#include <QObject>
#include <QPointF>
#include <QMutexLocker>

#include "map.h"

class DataManager : public QThread
{
    Q_OBJECT
    Q_PROPERTY(State state READ state NOTIFY stateChanged)
    Q_PROPERTY(QString saveGameFileName READ saveGameFileName NOTIFY saveGameFileNameChanged)

    Q_PROPERTY(QSize worldSize READ worldSize NOTIFY worldSizeChanged)
    Q_PROPERTY(QColor worldBackgroundColor READ worldBackgroundColor NOTIFY worldBackgroundColorChanged)

    Q_PROPERTY(Character *player READ player NOTIFY playerChanged)
    Q_PROPERTY(GameItems *items READ items CONSTANT)
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
    QString saveGameFileName() const;

    QSize worldSize() const;
    QColor worldBackgroundColor() const;

    Character *player() const;
    GameItems *items() const;
    GameItems *enemies() const;
    GameItems *characters() const;

    void createItem(const QString &resourcePath, const QPointF &position);

private:
    State m_state = StateIdle;
    QMutex m_stateMutex;

    QString m_saveGameFileName;
    QMutex m_saveGameFileNameMutex;

    QSize m_worldSize;
    QColor m_worldBackgroundColor = QColor("#307a78");

    Map *m_map = nullptr;
    QMutex m_mapMutex;

    Character *m_player = nullptr;
    QMutex m_playerMutex;

    GameItems *m_items = nullptr;
    GameItems *m_enemies = nullptr;
    GameItems *m_characters = nullptr;

    // Set members
    void setState(State state);
    void setWorldSize(const QSize &worlSize);
    void setWorldBackgroundColor(const QColor &color);
    void setSaveGameName(const QString &saveGameFileName);

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
    void saveGameFileNameChanged(const QString &saveGameFileName);

    void worldSizeChanged(const QSize &worldSize);
    void worldBackgroundColorChanged(const QColor &backgroundColor);
    void viewSizeChanged(const QSizeF &viewSize);
    void viewOffsetChanged(const QPointF &viewOffset);
    void playerChanged(Character *player);

private slots:
    void onThreadFinished();

public slots:
    void resetData();

    void startNewGame();
    void saveMap(Map *map, const QString &saveGameFileName);
    void saveGame(const QString &saveGameFileName);
    void loadGame(const QString &saveGameFileName);

};

#endif // DATAMANAGER_H
