#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QObject>
#include <QWindow>
#include <QSettings>

#include "gamewindow.h"

class GameSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(GameWindow::Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    Q_PROPERTY(QSize windowSize READ windowSize WRITE setWindowSize NOTIFY windowSizeChanged)
    Q_PROPERTY(QPoint windowPosition READ windowPosition WRITE setWindowPosition NOTIFY windowPositionChanged)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)

public:
    explicit GameSettings(QObject *parent = nullptr);

    // Game window
    GameWindow::Visibility visibility() const;
    void setVisibility(GameWindow::Visibility visibility);

    QSize windowSize() const;
    void setWindowSize(const QSize &windowSize);

    QPoint windowPosition() const;
    void setWindowPosition(const QPoint &windowPosition);

    // Player
    QString playerName() const;
    void setPlayerName(const QString &playerName);


private:
    QSettings *m_settings = nullptr;

    // Game window
    GameWindow::Visibility m_visibility;
    QSize m_windowSize;
    QPoint m_windowPosition;

    // Player
    QString m_playerName;

    // Init
    void loadSettings();

signals:
    // Game window
    void visibilityChanged(GameWindow::Visibility visibility);
    void windowSizeChanged(const QSize &windowSize);
    void windowPositionChanged(const QPoint &windowPosition);

    // Player
    void playerNameChanged(const QString &playerName);

};

#endif // GAMESETTINGS_H
