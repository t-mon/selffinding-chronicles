#include "gamesettings.h"
#include "debugcategories.h"

#include <QSettings>

GameSettings::GameSettings(QObject *parent) :
    QObject(parent)
{
    loadSettings();
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

    QSettings settings;
    settings.beginGroup("Player");
    settings.setValue("playerName", m_playerName);
    settings.endGroup();
}

QWindow::Visibility GameSettings::visibility() const
{
    return m_visibility;
}

void GameSettings::setVisibility(QWindow::Visibility visibility)
{
    if (m_visibility == visibility)
        return;

    qCDebug(dcSettings()) << "Visibility changed to" << visibility;
    m_visibility = visibility;
    emit visibilityChanged(m_visibility);

    QSettings settings;
    settings.beginGroup("Game");
    settings.setValue("visibility", static_cast<int>(visibility));
    settings.endGroup();
}

void GameSettings::loadSettings()
{
    QSettings settings;
    qCDebug(dcSettings()) << "Loading game settings from" << settings.fileName();

    settings.beginGroup("Game");

    m_visibility = static_cast<QWindow::Visibility>(settings.value("visibility", QWindow::Windowed).toInt());
    emit visibilityChanged(m_visibility);

    settings.endGroup();

    settings.beginGroup("Player");

    //: The default value for the player name
    m_playerName = settings.value("playerName", tr("Player")).toString();
    emit playerNameChanged(m_playerName);

    settings.endGroup();
}
