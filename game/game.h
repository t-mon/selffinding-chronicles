#ifndef GAME_H
#define GAME_H

#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QJSEngine>
#include <QQmlEngine>
#include <QElapsedTimer>

#include "gameworld.h"
#include "gamesettings.h"

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameWorld *world READ world CONSTANT)
    Q_PROPERTY(GameSettings *settings READ settings CONSTANT)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool debugging READ debugging WRITE setDebugging NOTIFY debuggingChanged)

public:
    static Game* instance();
    static QObject *qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    GameWorld *world() const;
    GameSettings *settings() const;

    bool running() const;
    void setRunning(const bool &running);

    bool debugging() const;
    void setDebugging(bool debugging);

    Q_INVOKABLE void keyPressed(const Qt::Key &key, bool autoRepeat);
    Q_INVOKABLE void keyReleased(const Qt::Key &key, bool autoRepeat);

private:
    explicit Game(QObject *parent = nullptr);
    static Game *s_instance;

    GameWorld *m_world = nullptr;
    GameSettings *m_settings = nullptr;
    quint32 m_currentTimer = 0;

    bool m_running = false;
    bool m_debugging = false;

signals:
    void tick();

    void runningChanged(bool running);
    void debuggingChanged(bool debugging);

public slots:
    void onTick();

};

#endif // GAME_H
