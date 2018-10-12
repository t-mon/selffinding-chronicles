#include "game.h"
#include "debugcategories.h"

Game* Game::s_instance = nullptr;

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

    qCDebug(dcGame()) << "The game is now" << (running ? "running" : "paused");
    emit runningChanged(m_running);
}

bool Game::debugging() const
{
    return m_debugging;
}

void Game::setDebugging(bool debugging)
{
    if (m_debugging == debugging)
        return;

    qCDebug(dcGame()) << "Debugging" << (debugging ? "enabled" : "disabled");
    m_debugging = debugging;
    emit debuggingChanged(m_debugging);
}

int Game::intervall() const
{
    return m_interval;
}

int Game::intermediateSteps() const
{
    return m_intermediateSteps;
}

void Game::keyPressed(const Qt::Key &key, bool autoRepeat)
{
    if (!running())
        return;

    if (autoRepeat)
        return;

    //qCDebug(dcGame()) << "Key pressed" << key;
    m_world->playerController()->keyPressed(key);

    switch (key) {
    case Qt::Key_NumberSign:
        setDebugging(!m_debugging);
        break;
    default:
        break;
    }
}

void Game::keyReleased(const Qt::Key &key, bool autoRepeat)
{
    if (!running())
        return;

    if (autoRepeat)
        return;

    //qCDebug(dcGame()) << "Key released" << key;
    m_world->playerController()->keyReleased(key);
}

Game::Game(QObject *parent) :
    QObject(parent),
    m_timer(new QTimer(this)),
    m_world(new World(this)),
    m_running(false),
    m_interval(5),
    m_intermediateSteps(30)
{
    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(m_interval);
    m_timer->setSingleShot(false);

    connect(m_timer, &QTimer::timeout, this, &Game::onTick);
}

void Game::onTick()
{
    m_tickCounter++;
    emit paintEvent();

    // Emit tick for the world
    m_world->tick();

    if (m_tickCounter >= m_intermediateSteps) {
        m_tickCounter = 0;

        // Emit tick for the UI
        emit tick();
    }
}
