#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QObject>
#include <QSettings>
#include <QQuickWindow>

#include "playercontroller.h"

class GameSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickWindow::Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(QPoint windowPosition READ windowPosition WRITE setWindowPosition NOTIFY windowPositionChanged)
    Q_PROPERTY(PlayerController::ControlMode controlMode READ controlMode WRITE setControlMode NOTIFY controlModeChanged)

    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)

public:
    explicit GameSettings(QObject *parent = nullptr);

    // Game window
    QQuickWindow::Visibility visibility() const;
    void setVisibility(QQuickWindow::Visibility visibility);

    QSize windowSize() const;
    void setWindowSize(const QSize &windowSize);

    QPoint windowPosition() const;
    void setWindowPosition(const QPoint &windowPosition);

    QString settingsPath() const;
    QString saveGamePath() const;
    QString mapEditorPath() const;

    // Game
    PlayerController::ControlMode controlMode() const;
    void setControlMode(PlayerController::ControlMode controlMode);

    // Player
    QString playerName() const;
    void setPlayerName(const QString &playerName);

private:
    QSettings *m_settings = nullptr;

    // Game window
    QQuickWindow::Visibility m_visibility;
    QSize m_windowSize;
    QPoint m_windowPosition;

    // Game
    PlayerController::ControlMode m_controlMode;

    // Player
    QString m_playerName;

    // Init
    void loadSettings();

signals:
    // Game window
    void visibilityChanged(QQuickWindow::Visibility visibility);
    void windowSizeChanged(const QSize &windowSize);
    void windowPositionChanged(const QPoint &windowPosition);

    // Game
    void controlModeChanged(PlayerController::ControlMode controlMode);

    // Player
    void playerNameChanged(const QString &playerName);

};

#endif // GAMESETTINGS_H
