#include "mapscene.h"
#include "debugcategories.h"

MapScene::MapScene(QObject *parent) : QObject(parent)
{
    // The visible and active items of the scene
    m_activeBackgroundLights = new GameObjectsProxy(this);
    m_activeBackgroundLights->setViewFilter(m_viewWindow);

    m_activeObjects = new GameObjectsProxy(this);
    m_activeObjects->setViewFilter(m_viewWindow);

    m_activeLights = new GameObjectsProxy(this);
    m_activeLights->setViewFilter(m_viewWindow);

    m_activeItems = new GameItemsProxy(this);
    m_activeItems->setViewFilter(m_viewWindow);

    m_activeChests = new GameItemsProxy(this);
    m_activeChests->setViewFilter(m_viewWindow);

    m_activeEnemies = new GameItemsProxy(this);
    m_activeEnemies->setViewFilter(m_viewWindow);

    m_activeCharacters = new GameItemsProxy(this);
    m_activeCharacters->setViewFilter(m_viewWindow);

    m_activeWeatherAreas = new WeatherAreaProxy(this);
    m_activeWeatherAreas->setViewFilter(m_viewWindow);

    connect(m_activeBackgroundLights, &GameObjectsProxy::gameObjectActiveChanged, this, &MapScene::onGameObjectActiveChanged);
    connect(m_activeObjects, &GameObjectsProxy::gameObjectActiveChanged, this, &MapScene::onGameObjectActiveChanged);
    connect(m_activeLights, &GameObjectsProxy::gameObjectActiveChanged, this, &MapScene::onGameObjectActiveChanged);

    connect(m_activeItems, &GameItemsProxy::gameItemActiveChanged, this, &MapScene::onGameItemActiveChanged);
    connect(m_activeChests, &GameItemsProxy::gameItemActiveChanged, this, &MapScene::onGameItemActiveChanged);
    connect(m_activeEnemies, &GameItemsProxy::gameItemActiveChanged, this, &MapScene::onGameItemActiveChanged);
    connect(m_activeCharacters, &GameItemsProxy::gameItemActiveChanged, this, &MapScene::onGameItemActiveChanged);
}

Map *MapScene::map()
{
    return m_map;
}

void MapScene::setMap(Map *map)
{
    if (m_map == map)
        return;

    m_map = map;

    if (m_map) {
        qCDebug(dcMapScene()) << "Set the new map to the scene" << m_map->name();

        // Initialize the proxies
        // Initially activate items around the player
        qCDebug(dcMapScene()) << "Initialize active objects and items around the player";
        if (m_map->player()) {
            QSize initialViewWindowSize(40, 40);
            QPointF initialViewWindowPosition(m_map->player()->position().x() - initialViewWindowSize.width() / 2, m_map->player()->position().y() - initialViewWindowSize.height() / 2);
            setViewWindow(QRectF(initialViewWindowPosition, initialViewWindowSize));
        }

        m_activeObjects->setGameObjects(m_map->objects());
        m_activeBackgroundLights->setGameObjects(m_map->backgroundLights());
        m_activeLights->setGameObjects(m_map->lights());
        m_activeItems->setGameItems(m_map->items());
        m_activeChests->setGameItems(m_map->chests());
        m_activeCharacters->setGameItems(m_map->characters());
        m_activeEnemies->setGameItems(m_map->enemies());
        m_activeWeatherAreas->setAreaModel(m_map->weatherAreas());

    } else {
        qCDebug(dcMapScene()) << "Reset all data and unset map";

        m_activeBackgroundLights->setGameObjects(nullptr);
        m_activeBackgroundLights->resetFilter();

        m_activeObjects->setGameObjects(nullptr);
        m_activeObjects->resetFilter();

        m_activeLights->setGameObjects(nullptr);
        m_activeLights->resetFilter();

        m_activeItems->setGameItems(nullptr);
        m_activeItems->resetFilter();

        m_activeChests->setGameItems(nullptr);
        m_activeChests->resetFilter();

        m_activeCharacters->setGameItems(nullptr);
        m_activeCharacters->resetFilter();

        m_activeEnemies->setGameItems(nullptr);
        m_activeEnemies->resetFilter();

        m_activeWeatherAreas->setAreaModel(nullptr);
        m_activeWeatherAreas->resetFilter();
    }

    emit mapChanged(m_map);
}

QRectF MapScene::viewWindow() const
{
    return m_viewWindow;
}

void MapScene::setViewWindow(const QRectF &viewWindow)
{
    if (m_viewWindow == viewWindow)
        return;

    m_viewWindow = viewWindow;
    emit viewWindowChanged(m_viewWindow);

    qCDebug(dcMapScene()) << "View window changed" << m_viewWindow;

    evaluateProxies();
}

GameObjectsProxy *MapScene::activeBackgroundLights() const
{
    return m_activeBackgroundLights;
}

GameObjectsProxy *MapScene::activeObjects() const
{
    return m_activeObjects;
}

GameObjectsProxy *MapScene::activeLights() const
{
    return m_activeLights;
}

GameItemsProxy *MapScene::activeItems() const
{
    return m_activeItems;
}

GameItemsProxy *MapScene::activeChests() const
{
    return m_activeChests;
}

GameItemsProxy *MapScene::activeCharacters() const
{
    return m_activeCharacters;
}

GameItemsProxy *MapScene::activeEnemies() const
{
    return m_activeEnemies;
}

WeatherAreaProxy *MapScene::activeWeatherAreas() const
{
    return m_activeWeatherAreas;
}

void MapScene::registerTemporaryLightSource(LightSource *lightSource)
{
    qCDebug(dcMapScene()) << "Register temporary light source";
    m_activeLights->gameObjects()->addGameObject(lightSource);
}

void MapScene::unregisterTemporaryLightSource(LightSource *lightSource)
{
    qCDebug(dcMapScene()) << "Unregister temporary light source";
    m_activeLights->gameObjects()->removeGameObject(lightSource);
}

void MapScene::evaluateProxies()
{
    m_activeBackgroundLights->setViewFilter(m_viewWindow);
    m_activeObjects->setViewFilter(m_viewWindow);
    m_activeLights->setViewFilter(m_viewWindow);
    m_activeItems->setViewFilter(m_viewWindow);
    m_activeChests->setViewFilter(m_viewWindow);
    m_activeEnemies->setViewFilter(m_viewWindow);
    m_activeCharacters->setViewFilter(m_viewWindow);
    m_activeWeatherAreas->setViewFilter(m_viewWindow);
}

void MapScene::onGameObjectActiveChanged(GameObject *object, bool active)
{
    if (active) {
        qCDebug(dcMapScene()) << "[+] Object changed to active" << object;
    } else {
        qCDebug(dcMapScene()) << "[-] Object changed to inactive" << object;
    }
    object->setActive(active);
}

void MapScene::onGameItemActiveChanged(GameItem *item, bool active)
{
    if (active) {
        qCDebug(dcMapScene()) << "[+] Item changed to active" << item;
    } else {
        qCDebug(dcMapScene()) << "[-] Item changed to inactive" << item;
    }
    item->setActive(active);
}
