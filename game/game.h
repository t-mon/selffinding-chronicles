#ifndef GAME_H
#define GAME_H

#include <QTimer>
#include <QObject>
#include <QKeyEvent>
#include <QJSEngine>
#include <QQmlEngine>
#include <QElapsedTimer>

#include "engine.h"
#include "gamesettings.h"
#include "gamemapeditor.h"
#include "audio/soundengine.h"

class Game : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Engine *engine READ engine CONSTANT)
    Q_PROPERTY(GameSettings *settings READ settings CONSTANT)
    Q_PROPERTY(GameMapEditor *mapEditor READ mapEditor CONSTANT)

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
    Q_PROPERTY(bool debugging READ debugging WRITE setDebugging NOTIFY debuggingChanged)

public:
    enum GamePlatform {
        GamePlatformDesktop,
        GamePlatformMobile
    };
    Q_ENUM(GamePlatform)

    static Game* instance();
    static QObject *qmlInstance(QQmlEngine *qmlEngine, QJSEngine *jsEngine);

    Engine *engine() const;
    GameSettings *settings() const;
    GameMapEditor *mapEditor() const;

    bool running() const;
    void setRunning(const bool &running);

    bool debugging() const;
    void setDebugging(bool debugging);

    Q_INVOKABLE void keyPressed(const Qt::Key &key, bool autoRepeat);
    Q_INVOKABLE void keyReleased(const Qt::Key &key, bool autoRepeat);

    // Cast methods
    Q_INVOKABLE WeaponItem *castWeaponItem(GameItem *item);
    Q_INVOKABLE FirearmItem *castFirearmItem(GameItem *item);
    Q_INVOKABLE PlantItem *castPlantItem(GameItem *item);
    Q_INVOKABLE LiteratureItem *castLiteratureItem(GameItem *item);
    Q_INVOKABLE TeleporterItem *castTeleporterItem(GameItem *item);
    Q_INVOKABLE Character *castEnemyToCharacter(Enemy *enemy);

private:
    explicit Game(QObject *parent = nullptr);
    static Game *s_instance;

    Engine *m_engine = nullptr;
    SoundEngine *m_soundEngine = nullptr;
    GameSettings *m_settings = nullptr;
    GameMapEditor *m_mapEditor = nullptr;

    quint32 m_currentTimer = 0;

    bool m_running = false;
    bool m_debugging = false;

signals:
    void tick();

    void runningChanged(bool running);
    void debuggingChanged(bool debugging);

public slots:
    void onTick();

};

#endif // GAME_H
