#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QTimer>
#include <QKeyEvent>

#include "world.h"
#include "gamesettings.h"

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(World *world READ world CONSTANT)
    Q_PROPERTY(GameSettings *settings READ settings CONSTANT)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool debugging READ debugging WRITE setDebugging NOTIFY debuggingChanged)

public:

    friend class GameWindow;

    enum GameState {
        Uninitialized,
        Loading,
        Paused,
        Running,
        Inventory,
        Conversation
    };
    Q_ENUM(GameState)

    static Game* instance();
    static void destroy();

    static QObject *qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    World *world() const;
    GameSettings *settings() const;

    bool running() const;
    void setRunning(const bool &running);

    bool debugging() const;
    void setDebugging(bool debugging);

    GameState state() const;
    void setState(GameState state);

    void keyPressed(const Qt::Key &key, bool autoRepeat);
    void keyReleased(const Qt::Key &key, bool autoRepeat);

private:
    explicit Game(QObject *parent = nullptr);
    static Game *s_instance;

    QTimer *m_timer = nullptr;
    World *m_world = nullptr;
    GameSettings *m_settings = nullptr;

    bool m_running = false;
    bool m_debugging = false;
    GameState m_state = Uninitialized;

    int m_interval = 0;
    int m_intermediateSteps = 0;
    int m_tickCounter = 0;

signals:
    void tick();
    void paintEvent();

    void runningChanged(bool running);
    void debuggingChanged(bool debugging);
    void stateChanged(GameState state);

private slots:
    void gameTick();

};

#endif // GAME_H
