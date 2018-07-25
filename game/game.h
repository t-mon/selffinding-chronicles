#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QQmlEngine>
#include <QJSEngine>
#include <QTimer>
#include <QKeyEvent>

#include "world.h"
#include "player.h"

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(World *world READ world CONSTANT)
    Q_PROPERTY(int intervall READ intervall CONSTANT)
    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool debugging READ debugging WRITE setDebugging NOTIFY debuggingChanged)

public:
    static Game* instance();
    static QObject *qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    World *world();

    bool running() const;
    void setRunning(const bool &running);

    bool debugging() const;
    void setDebugging(bool debugging);

    int intervall() const;
    int intermediateSteps() const;

    Q_INVOKABLE void keyPressed(const Qt::Key &key, bool autoRepeat);
    Q_INVOKABLE void keyReleased(const Qt::Key &key, bool autoRepeat);

private:
    explicit Game(QObject *parent = nullptr);
    static Game *s_instance;

    QTimer *m_timer = nullptr;

    World *m_world;
    bool m_running = false;
    bool m_debugging = false;
    int m_interval = 0;
    int m_intermediateSteps = 0;
    int m_tickCounter = 0;

signals:
    void tick();
    void paintEvent();

    void runningChanged(bool running);
    void debuggingChanged(bool debugging);

private slots:
    void onTick();

};

#endif // GAME_H
