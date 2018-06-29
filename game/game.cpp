#include "game.h"

#include <QDebug>

Game* Game::s_instance = 0;

Game *Game::instance()
{
    if (!s_instance)
        s_instance = new Game();

    return s_instance;
}

QObject *Game::qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    return Game::instance();
}

World *Game::world()
{
    return m_world;
}

Game::ControlMode Game::controlMode() const
{
    return m_controlMode;
}

void Game::setControlMode(const Game::ControlMode &controlMode)
{
    if (m_controlMode != controlMode) {
        m_controlMode = controlMode;
        emit controlModeChanged(m_controlMode);
        qDebug() << m_controlMode;
    }
}

bool Game::running() const
{
    return m_running;
}

void Game::setRunning(const bool &running)
{
    if (m_running == running)
        return;

    m_running = running;

    if (m_running)
        m_timer->start();
    else
        m_timer->stop();

    qDebug() << "Game" << (running ? "running" : "paused");
    emit runningChanged(m_running);
}

int Game::intervall() const
{
    return 150;
}

void Game::keyPressed(const Qt::Key &key)
{
    //qDebug() << "Key pressed" << key;

    if (!running())
        return;

    switch (key) {
    case Qt::Key_W:
        m_world->setForwardPressed(true);
        break;
    case Qt::Key_S:
        m_world->setBackwardPressed(true);
        break;
    case Qt::Key_A:
        m_world->setLeftPressed(true);
        break;
    case Qt::Key_D:
        m_world->setRightPressed(true);
        break;
    default:
        break;
    }
}

void Game::keyReleased(const Qt::Key &key)
{
    //qDebug() << "Key released" << key;

    if (!running())
        return;

    switch (key) {
    case Qt::Key_W:
        m_world->setForwardPressed(false);
        break;
    case Qt::Key_S:
        m_world->setBackwardPressed(false);
        break;
    case Qt::Key_A:
        m_world->setLeftPressed(false);
        break;
    case Qt::Key_D:
        m_world->setRightPressed(false);
        break;
    default:
        break;
    }
}

Game::Game(QObject *parent) :
    QObject(parent),
    m_timer(new QTimer(this)),
    m_world(new World(this)),
    m_controlMode(ControlModeKeyBoard),
    m_running(false)
{
    m_timer->setInterval(intervall());
    m_timer->setSingleShot(false);

    connect(m_timer, &QTimer::timeout, this, &Game::onTick);
}

void Game::onTick()
{
    // Emit tick for the world
    m_world->tick();

    // Emit tick for the UI
    emit tick();
}

