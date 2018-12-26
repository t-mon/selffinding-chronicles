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

GameWorld *Game::world()
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

    if (m_running) {
        //m_timer->start();
        setState(GameState::Running);
    } else {
        //m_timer->stop();
        setState(GameState::Paused);
    }

    //qCDebug(dcGame()) << "The game is now" << (running ? "running" : "paused");
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

Game::GameState Game::state() const
{
    return m_state;
}

void Game::setState(Game::GameState state)
{
    if (m_state == state)
        return;

    qCDebug(dcGame()) << state;
    m_state = state;
    emit stateChanged(m_state);

    // TODO: switch state
}

int Game::intervall() const
{
    return m_interval;
}

void Game::keyPressed(const Qt::Key &key, bool autoRepeat)
{
    if (autoRepeat)
        return;

    qCDebug(dcGame()) << "Key pressed" << key;
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
    if (autoRepeat)
        return;

    qCDebug(dcGame()) << "Key released" << key;
    m_world->playerController()->keyReleased(key);
}

Game::Game(QObject *parent) :
    QObject(parent),
    m_timer(new QTimer(this)),
    m_world(new GameWorld(this)),
    m_settings(new GameSettings(this)),
    m_running(false)
{
    m_interval = qRound(1000.0 / m_fps);

    m_timer->setTimerType(Qt::PreciseTimer);
    m_timer->setInterval(m_interval);
    m_timer->setSingleShot(false);

    qCDebug(dcGame()) << "FPS:" << m_fps << "[ms] interval:" << m_interval << "[ms]";
    connect(m_timer, &QTimer::timeout, this, &Game::onTick, Qt::DirectConnection);
}

void Game::onTick()
{
    //qCDebug(dcGame()) << "Tick" << m_elapsedTimer.elapsed() << "ms";
    m_world->tick();
    //m_elapsedTimer.restart();
}
