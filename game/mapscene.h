#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QObject>

#include "map.h"
#include "gameobjectsproxy.h"
#include "items/gameitemsproxy.h"
#include "items/weatherareaproxy.h"

class MapScene : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Map *map READ map WRITE setMap NOTIFY mapChanged)
    Q_PROPERTY(QRectF viewWindow READ viewWindow WRITE setViewWindow NOTIFY viewWindowChanged)

    Q_PROPERTY(GameObjectsProxy *activeBackgroundLights READ activeBackgroundLights CONSTANT)
    Q_PROPERTY(GameObjectsProxy *activeObjects READ activeObjects CONSTANT)
    Q_PROPERTY(GameObjectsProxy *activeLights READ activeLights CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeItems READ activeItems CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeChests READ activeChests CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeEnemies READ activeEnemies CONSTANT)
    Q_PROPERTY(GameItemsProxy *activeCharacters READ activeCharacters CONSTANT)
    Q_PROPERTY(WeatherAreaProxy *activeWeatherAreas READ activeWeatherAreas CONSTANT)



public:
    explicit MapScene(QObject *parent = nullptr);

    Map *map();
    void setMap(Map *map);

    QRectF viewWindow() const;
    void setViewWindow(const QRectF &viewWindow);

    GameObjectsProxy *activeBackgroundLights() const;
    GameObjectsProxy *activeObjects() const;
    GameObjectsProxy *activeLights() const;
    GameItemsProxy *activeItems() const;
    GameItemsProxy *activeChests() const;
    GameItemsProxy *activeCharacters() const;
    GameItemsProxy *activeEnemies() const;
    WeatherAreaProxy *activeWeatherAreas() const;


    Q_INVOKABLE void registerTemporaryLightSource(LightSource *lightSource);
    Q_INVOKABLE void unregisterTemporaryLightSource(LightSource *lightSource);

signals:
    void mapChanged(Map *map);
    void viewWindowChanged(const QRectF &viewWindow);

private:
    QRectF m_viewWindow = QRectF(-1, -1, 1, 1); // is valid, therefore it will be used as filter
    Map *m_map = nullptr;

    GameObjectsProxy *m_activeBackgroundLights = nullptr;
    GameObjectsProxy *m_activeObjects = nullptr;
    GameObjectsProxy *m_activeLights = nullptr;
    GameItemsProxy *m_activeItems = nullptr;
    GameItemsProxy *m_activeChests = nullptr;
    GameItemsProxy *m_activeEnemies = nullptr;
    GameItemsProxy *m_activeCharacters = nullptr;
    WeatherAreaProxy *m_activeWeatherAreas = nullptr;

    void evaluateProxies();

private slots:
    void onGameObjectActiveChanged(GameObject *object, bool active);
    void onGameItemActiveChanged(GameItem *item, bool active);


};

#endif // MAPSCENE_H
