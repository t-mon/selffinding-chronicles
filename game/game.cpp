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

Engine *Game::engine() const
{
    return m_engine;
}

GameSettings *Game::settings() const
{
    return m_settings;
}

GameMapEditor *Game::mapEditor() const
{
    return m_mapEditor;
}

bool Game::running() const
{
    return m_running;
}

void Game::setRunning(const bool &running)
{
    if (m_running == running)
        return;

    qCDebug(dcGame()) << "The game is now" << (running ? "running" : "paused");
    m_running = running;
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

void Game::keyPressed(const Qt::Key &key, bool autoRepeat)
{
    if (autoRepeat)
        return;

    qCDebug(dcGame()) << "Key pressed" << key;
    m_engine->playerController()->keyPressed(key);

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
    m_engine->playerController()->keyReleased(key);
}

WeaponItem *Game::castWeaponItem(GameItem *item)
{
    return qobject_cast<WeaponItem *>(item);
}

FirearmItem *Game::castFirearmItem(GameItem *item)
{
    return qobject_cast<FirearmItem *>(item);
}

PlantItem *Game::castPlantItem(GameItem *item)
{
    return qobject_cast<PlantItem *>(item);
}

Game::Game(QObject *parent) :
    QObject(parent),
    m_engine(new Engine(this)),
    m_settings(new GameSettings(this)),
    m_mapEditor(new GameMapEditor(this)),
    m_running(false)
{

}

void Game::onTick()
{
    m_engine->tick();
}
