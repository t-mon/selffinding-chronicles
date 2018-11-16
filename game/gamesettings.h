#ifndef GAMESETTINGS_H
#define GAMESETTINGS_H

#include <QObject>
#include <QWindow>

class GameSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QWindow::Visibility visibility READ visibility WRITE setVisibility NOTIFY visibilityChanged)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)


public:
    explicit GameSettings(QObject *parent = nullptr);

    QString playerName() const;
    void setPlayerName(const QString &playerName);

    QWindow::Visibility visibility() const;
    void setVisibility(QWindow::Visibility visibility);


private:
    // Game
    QWindow::Visibility m_visibility;

    // Player
    QString m_playerName;

    void loadSettings();

signals:
    void visibilityChanged(QWindow::Visibility visibility);
    void playerNameChanged(const QString &playerName);

};

#endif // GAMESETTINGS_H
