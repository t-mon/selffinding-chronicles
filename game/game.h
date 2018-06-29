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
    Q_PROPERTY(ControlMode controlMode READ controlMode WRITE setControlMode NOTIFY controlModeChanged)

public:
    enum ControlMode {
        ControlModeKeyBoard,
        ControlModeKeyBoardMouse,
        ControlModeTouchscreen
    };
    Q_ENUM(ControlMode)

    static Game* instance();
    static QObject *qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    World *world();

    ControlMode controlMode() const;
    void setControlMode(const ControlMode &controlMode);

    bool running() const;
    void setRunning(const bool &running);

    int intervall() const;

    Q_INVOKABLE void keyPressed(const Qt::Key &key);
    Q_INVOKABLE void keyReleased(const Qt::Key &key);

private:
    explicit Game(QObject *parent = nullptr);
    static Game *s_instance;

    QTimer *m_timer;
    World *m_world;
    ControlMode m_controlMode;
    bool m_running;

signals:
    void tick();
    void runningChanged(const bool &running);
    void controlModeChanged(const bool &controlMode);

private slots:
    void onTick();

};

#endif // GAME_H
