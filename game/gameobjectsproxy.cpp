#include "gameobjectsproxy.h"

GameObjectsProxy::GameObjectsProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    connect(this, &GameObjectsProxy::rowsInserted, this, &GameObjectsProxy::onRowsInseted);
    connect(this, &GameObjectsProxy::rowsRemoved, this, &GameObjectsProxy::onRowsRemoved);
    connect(this, &GameObjectsProxy::rowsAboutToBeRemoved, this, &GameObjectsProxy::onRowsAboutToBeRemoved);
}

GameObjects *GameObjectsProxy::gameObjects() const
{
    return m_gameObjects;
}

void GameObjectsProxy::setGameObjects(GameObjects *gameObjects)
{
    if (m_gameObjects == gameObjects)
        return;

    // Disconnect old model
    if (m_gameObjects)
        disconnect(m_gameObjects, &GameObjects::countChanged, this, &GameObjectsProxy::onSourceModelCountChanged);

    beginResetModel();
    m_gameObjects = gameObjects;
    setSourceModel(m_gameObjects);
    emit gameObjectsChanged(gameObjects);

    if (m_gameObjects) {
        connect(m_gameObjects, &GameObjects::countChanged, this, &GameObjectsProxy::onSourceModelCountChanged);
    }

    endResetModel();

    invalidateFilter();
    sort(0);
    emit countChanged();
}

int GameObjectsProxy::count() const
{
    return rowCount(QModelIndex());
}

QRectF GameObjectsProxy::viewFilter() const
{
    return m_viewFilter;
}

void GameObjectsProxy::setViewFilter(const QRectF &viewFilter)
{
    if (m_viewFilter == viewFilter)
        return;

    m_viewFilter = viewFilter;
    emit viewFilterChanged(m_viewFilter);

    invalidateFilter();
    emit countChanged();
}

GameObject *GameObjectsProxy::get(int index) const
{
    if (!m_gameObjects)
        return nullptr;

    return m_gameObjects->get(mapToSource(this->index(index, 0)).row());
}

void GameObjectsProxy::resetFilter()
{
    setViewFilter(QRectF());
}

bool GameObjectsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    // Get the current game item
    if (!m_gameObjects)
        return false;

    GameObject *gameObject = m_gameObjects->get(sourceRow);
    if (!gameObject)
        return false;

    if (m_viewFilter.isValid()) {
        if (!m_viewFilter.intersects(QRectF(gameObject->position(), gameObject->size()))) {
            if (gameObject->active()) {
                emit const_cast<GameObjectsProxy *>(this)->gameObjectActiveChanged(gameObject, false);
            }
            return false;
        } else {
            if (!gameObject->active()) {
                emit const_cast<GameObjectsProxy *>(this)->gameObjectActiveChanged(gameObject, true);
            }
        }
    }

    return true;
}

bool GameObjectsProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    // Alphabetically for now
    QVariant leftName = sourceModel()->data(left);
    QVariant rightName = sourceModel()->data(right);

    return QString::localeAwareCompare(leftName.toString(), rightName.toString()) < 0;
}

void GameObjectsProxy::onSourceModelCountChanged()
{
    invalidateFilter();
    sort(0);
    emit countChanged();
}

void GameObjectsProxy::onRowsInseted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    if (m_viewFilter.isValid()) {
        for (int i = first; i <= last; i++) {
            GameObject *object = get(i);
            if (object && !object->active()) {
                emit gameObjectActiveChanged(object, true);
            }
        }
    }

    emit countChanged();
}

void GameObjectsProxy::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    emit countChanged();
}

void GameObjectsProxy::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    if (m_viewFilter.isValid()) {
        for (int i = first; i <= last; i++) {
            GameObject *object = get(i);
            if (object && object->active()) {
                emit gameObjectActiveChanged(object, false);
            }
        }
    }

    emit countChanged();
}
