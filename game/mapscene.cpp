#include "mapscene.h"
#include "debugcategories.h"

MapScene::MapScene(QObject *parent) : QObject(parent)
{
    // The visible and active items of the scene
    m_activeBackgroundLights = new GameObjectsProxy(this);
    m_activeBackgroundLights->setViewFilter(m_viewWindow);

    m_activeObjects = new GameObjectsProxy(this);
    m_activeObjects->setViewFilter(m_viewWindow);

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
    emit mapChanged(m_map);

    if (m_map) {
        qCDebug(dcMapScene()) << "Set the new map to the scene" << m_map->name();

        qCDebug(dcMapScene()) << "Initialize active objects and items";

        // Initialize the proxies
        m_activeBackgroundLights->setGameObjects(m_map->backgroundLights());
        m_activeObjects->setGameObjects(m_map->objects());
        m_activeItems->setGameItems(m_map->items());
        m_activeChests->setGameItems(m_map->chests());
        m_activeCharacters->setGameItems(m_map->characters());
        m_activeEnemies->setGameItems(m_map->enemies());
        m_activeWeatherAreas->setAreaModel(m_map->weatherAreas());

        // Default view window for preloading items in the scene
        QSizeF windowSize(30, 30);
        if (m_map->player()) {
            setViewWindow(QRectF(QPointF(m_map->player()->centerPosition().x() - windowSize.width() / 2,
                                         m_map->player()->centerPosition().y() - windowSize.height() / 2), windowSize));
        } else {
            setViewWindow(QRectF(QPointF(0, 0), windowSize));
        }

        // Activate all objects
        qCDebug(dcMapScene()) << "Active background lights" << m_activeBackgroundLights->count();
        for (int i = 0; i < m_activeObjects->count(); i++) {
            m_activeObjects->get(i)->setActive(true);
        }

        qCDebug(dcMapScene()) << "Active objects" << m_activeObjects->count();
        for (int i = 0; i < m_activeObjects->count(); i++) {
            m_activeObjects->get(i)->setActive(true);
        }

        qCDebug(dcMapScene()) << "Active items" << m_activeItems->count();
        for (int i = 0; i < m_activeItems->count(); i++) {
            m_activeItems->get(i)->setActive(true);
        }

        qCDebug(dcMapScene()) << "Active chests" << m_activeChests->count();
        for (int i = 0; i < m_activeChests->count(); i++) {
            m_activeChests->get(i)->setActive(true);
        }

        qCDebug(dcMapScene()) << "Active characters" << m_activeCharacters->count();
        for (int i = 0; i < m_activeCharacters->count(); i++) {
            m_activeCharacters->get(i)->setActive(true);
        }

        qCDebug(dcMapScene()) << "Active enemies" << m_activeEnemies->count();
        for (int i = 0; i < m_activeEnemies->count(); i++) {
            m_activeEnemies->get(i)->setActive(true);
        }

        qCDebug(dcMapScene()) << "Active weather areas" << m_activeWeatherAreas->count();

    } else {
        qCDebug(dcMapScene()) << "Reset. No map set to scene";
    }
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

void MapScene::evaluateProxies()
{
    m_activeBackgroundLights->setViewFilter(m_viewWindow);
    m_activeObjects->setViewFilter(m_viewWindow);
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
