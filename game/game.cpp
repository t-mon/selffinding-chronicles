#include "game.h"
#include "debugcategories.h"

Game* Game::s_instance = nullptr;

Game *Game::instance()
{
    if (!s_instance) {
        qCDebug(dcGame()) << "Create game instance";
        s_instance = new Game();
    }
    return s_instance;
}

void Game::destroy()
{
    qCDebug(dcGame()) << "Destroy game instance";
    if (s_instance) {
        delete s_instance;
        s_instance = nullptr;
    }
}

QObject *Game::qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    return Game::instance();
}

World *Game::world() const
{
    return m_world;
}

GameSettings *Game::settings() const
{
    return m_settings;
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
        m_timer->start();
        setState(GameState::Running);
    } else {
        m_timer->stop();
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

void Game::keyPressed(const Qt::Key &key, bool autoRepeat)
{
    if (autoRepeat)
        return;

    qCDebug(dcGame()) << "Key pressed" << key;
    m_world->playerController()->keyPressed(key);
    gameTick();

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
    gameTick();
}

Game::Game(QObject *parent) :
    QObject(parent),
    m_timer(new QTimer(this)),
    m_world(new World(this)),
    m_settings(new GameSettings(this)),
    m_running(false)
{
    // Init settings
    m_world->player()->setName(m_settings->playerName());
}

void Game::gameTick()
{
    if (!m_running)
        return;

    m_world->tick();
}
