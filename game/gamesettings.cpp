#include "gamesettings.h"
#include "debugcategories.h"

#include <QDir>
#include <QFile>

GameSettings::GameSettings(QObject *parent) :
    QObject(parent)
{
    m_settings = new QSettings("self-finding-cronicles", "self-finding-cronicles", this);
    loadSettings();
}

QQuickWindow::Visibility GameSettings::visibility() const
{
    return m_visibility;
}

void GameSettings::setVisibility(QQuickWindow::Visibility visibility)
{
    if (m_visibility == visibility)
        return;

    qCDebug(dcSettings()) << "Visibility changed to" << visibility;
    m_visibility = visibility;
    emit visibilityChanged(m_visibility);

    m_settings->beginGroup("Game");
    m_settings->beginGroup("window");
    m_settings->setValue("visibility", static_cast<int>(m_visibility));
    m_settings->endGroup(); // window
    m_settings->endGroup(); // Game
}

QSize GameSettings::windowSize() const
{
    return m_windowSize;
}

void GameSettings::setWindowSize(const QSize &windowSize)
{
    if (m_windowSize == windowSize)
        return;

    qCDebug(dcSettings()) << "Window size changed to" << windowSize;
    m_windowSize = windowSize;
    emit windowSizeChanged(m_windowSize);

    m_settings->beginGroup("Game");
    m_settings->beginGroup("window");
    m_settings->setValue("size", m_windowSize);
    m_settings->endGroup(); // window
    m_settings->endGroup(); // Game
}

QPoint GameSettings::windowPosition() const
{
    return m_windowPosition;
}

void GameSettings::setWindowPosition(const QPoint &windowPosition)
{
    if (m_windowPosition == windowPosition)
        return;

    qCDebug(dcSettings()) << "Window position changed to" << windowPosition;
    m_windowPosition = windowPosition;
    emit windowPositionChanged(m_windowPosition);

    m_settings->beginGroup("Game");
    m_settings->beginGroup("window");
    m_settings->setValue("position", m_windowPosition);
    m_settings->endGroup(); // window
    m_settings->endGroup(); // Game
}

QString GameSettings::settingsPath() const
{
    QFileInfo settingsFileInfo(m_settings->fileName());
    return settingsFileInfo.canonicalPath();
}

QString GameSettings::saveGamePath() const
{
    return settingsPath() + QDir::separator() + "savegames";
}

QString GameSettings::mapEditorPath() const
{
    return settingsPath() + QDir::separator() + "mapeditor";
}

PlayerController::ControlMode GameSettings::controlMode() const
{
    return m_controlMode;
}

void GameSettings::setControlMode(PlayerController::ControlMode controlMode)
{
    if (m_controlMode == controlMode)
        return;

    qCDebug(dcSettings()) << "Control mode changed to" << controlMode;
    m_controlMode = controlMode;
    emit controlModeChanged(m_controlMode);

    m_settings->beginGroup("Game");
    m_settings->setValue("controlMode", m_controlMode);
    m_settings->endGroup(); // Game
}

QString GameSettings::playerName() const
{
    return m_playerName;
}

void GameSettings::setPlayerName(const QString &playerName)
{
    if (m_playerName == playerName)
        return;

    qCDebug(dcSettings()) << "Player name changed to" << playerName;
    m_playerName = playerName;
    emit playerNameChanged(m_playerName);

    m_settings->beginGroup("Player");
    m_settings->setValue("playerName", m_playerName);
    m_settings->endGroup();
}

bool GameSettings::gridSnapping() const
{
    return m_gridSnapping;
}

void GameSettings::setGridSnipping(bool gridSnapping)
{
    if (m_gridSnapping == gridSnapping)
        return;

    qCDebug(dcSettings()) << "Map editor grid snipping changed to" << gridSnapping;
    m_gridSnapping = gridSnapping;
    emit gridSnappingChanged(m_gridSnapping);

    m_settings->beginGroup("MapEditor");
    m_settings->setValue("gridSnapping", m_gridSnapping);
    m_settings->endGroup();

}

bool GameSettings::itemDebugEnabled() const
{
    return m_itemDebugEnabled;
}

void GameSettings::setItemDebugEnabled(bool itemDebugEnabled)
{
    if (m_itemDebugEnabled == itemDebugEnabled)
        return;

    qCDebug(dcSettings()) << "Map editor item debug changed to" << itemDebugEnabled;
    m_itemDebugEnabled = itemDebugEnabled;
    emit itemDebugEnabledChanged(m_itemDebugEnabled);

    m_settings->beginGroup("MapEditor");
    m_settings->setValue("itemDebugEnabled", m_itemDebugEnabled);
    m_settings->endGroup();
}

bool GameSettings::physicsDebugEnabled() const
{
    return m_physicsDebugEnabled;
}

void GameSettings::setPhysicsDebugEnabled(bool physicsDebugEnabled)
{
    if (m_physicsDebugEnabled == physicsDebugEnabled)
        return;

    qCDebug(dcSettings()) << "Map editor physics debug changed to" << physicsDebugEnabled;
    m_physicsDebugEnabled = physicsDebugEnabled;
    emit physicsDebugEnabledChanged(m_physicsDebugEnabled);

    m_settings->beginGroup("MapEditor");
    m_settings->setValue("physicsDebugEnabled", m_physicsDebugEnabled);
    m_settings->endGroup();
}

void GameSettings::loadSettings()
{
    qCDebug(dcSettings()) << "Loading game settings from" << m_settings->fileName();

    m_settings->beginGroup("Game");

#ifdef ANDROID
    m_controlMode = static_cast<PlayerController::ControlMode>(m_settings->value("controlMode", PlayerController::ControlModeTouchscreen).toInt());
#else
    m_controlMode = static_cast<PlayerController::ControlMode>(m_settings->value("controlMode", PlayerController::ControlModeKeyBoard).toInt());
#endif
    emit controlModeChanged(m_controlMode);

    m_settings->beginGroup("window");

    m_windowSize = m_settings->value("size", QSize(800, 600)).toSize();
    emit windowSizeChanged(m_windowSize);

    m_windowPosition = m_settings->value("position", QPoint(0, 0)).toPoint();
    emit windowPositionChanged(m_windowPosition);

#ifdef ANDROID
    m_visibility = static_cast<QQuickWindow::Visibility>(m_settings->value("visibility", QQuickWindow::FullScreen).toInt());
#else
    m_visibility = static_cast<QQuickWindow::Visibility>(m_settings->value("visibility", QQuickWindow::Windowed).toInt());
#endif
    emit visibilityChanged(m_visibility);

    m_settings->endGroup(); // window
    m_settings->endGroup(); // Game

    m_settings->beginGroup("Player");
    //: The default value for the player name
    m_playerName = m_settings->value("playerName", tr("Player")).toString();
    emit playerNameChanged(m_playerName);
    m_settings->endGroup(); // Player

    m_settings->beginGroup("MapEditor");
    //: The default value for the player name
    m_gridSnapping = m_settings->value("gridSnapping", true).toBool();
    emit gridSnappingChanged(m_gridSnapping);
    m_itemDebugEnabled = m_settings->value("itemDebugEnabled", false).toBool();
    emit itemDebugEnabledChanged(m_itemDebugEnabled);
    m_physicsDebugEnabled = m_settings->value("physicsDebugEnabled", false).toBool();
    emit physicsDebugEnabledChanged(m_physicsDebugEnabled);
    m_settings->endGroup(); // MapEditor

}
