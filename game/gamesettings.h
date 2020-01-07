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

    Q_PROPERTY(bool gridSnapping READ gridSnapping WRITE setGridSnipping NOTIFY gridSnappingChanged)
    Q_PROPERTY(bool itemDebugEnabled READ itemDebugEnabled WRITE setItemDebugEnabled NOTIFY itemDebugEnabledChanged)
    Q_PROPERTY(bool physicsDebugEnabled READ physicsDebugEnabled WRITE setPhysicsDebugEnabled NOTIFY physicsDebugEnabledChanged)

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

    // Map Editor
    bool gridSnapping() const;
    void setGridSnipping(bool gridSnapping);

    bool itemDebugEnabled() const;
    void setItemDebugEnabled(bool itemDebugEnabled);

    bool physicsDebugEnabled() const;
    void setPhysicsDebugEnabled(bool physicsDebugEnabled);

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

    // Map Editor
    bool m_gridSnapping = false;
    bool m_itemDebugEnabled = false;
    bool m_physicsDebugEnabled = false;

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

    // Map Editor
    void gridSnappingChanged(bool gridSnapping);
    void itemDebugEnabledChanged(bool itemDebugEnabled);
    void physicsDebugEnabledChanged(bool physicsDebugEnabled);

};

#endif // GAMESETTINGS_H
