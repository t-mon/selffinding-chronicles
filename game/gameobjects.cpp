#include "gameobjects.h"
#include "debugcategories.h"

GameObjects::GameObjects(QObject *parent) :
    QAbstractListModel(parent)
{

}

QList<GameObject *> GameObjects::gameObjects() const
{
    return m_gameObjects;
}

GameObject *GameObjects::get(int index)
{
    if (index >= m_gameObjects.count() || index < 0)
        return nullptr;

    return m_gameObjects.at(index);
}

int GameObjects::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_gameObjects.count();
}

QVariant GameObjects::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_gameObjects.count())
        return QVariant();

    GameObject *gameObject = m_gameObjects.at(index.row());
    if (!gameObject)
        return QVariant();

    if (role == PositionRole) {
        return gameObject->position();
    } else if (role == NameRole) {
        return gameObject->name();
    } else if (role == CenterPositionRole) {
        return gameObject->centerPosition();
    } else if (role == SizeRole) {
        return gameObject->size();
    } else if (role == ImageNameRole) {
        return gameObject->imageName();
    } else if (role == ResourcePathRole) {
        return gameObject->resourcePath();
    } else if (role == ActiveRole) {
        return gameObject->active();
    } else if (role == LayerRole) {
        return static_cast<int>(gameObject->layer());
    }

    return QVariant();
}

void GameObjects::addGameObject(GameObject *object)
{
    if (m_gameObjects.contains(object))
        return;

    object->setParent(this);

    beginInsertRows(QModelIndex(), m_gameObjects.count(), m_gameObjects.count());
    m_gameObjects.append(object);
    endInsertRows();

    emit countChanged(m_gameObjects.count());
}

void GameObjects::addGameObjectList(QList<GameObject *> objectList)
{
    foreach (GameObject *object, objectList) {
        addGameObject(object);
    }
}

void GameObjects::removeGameObject(GameObject *object)
{
    if (!m_gameObjects.contains(object))
        return;

    int index = m_gameObjects.indexOf(object);
    beginRemoveRows(QModelIndex(), index, index);
    m_gameObjects.removeAt(index);
    endRemoveRows();

    emit countChanged(m_gameObjects.count());
}

void GameObjects::clearModel()
{
    beginResetModel();
    foreach (GameObject *object, m_gameObjects) {
        object->deleteLater();
    }
    m_gameObjects.clear();
    endResetModel();

    emit countChanged(m_gameObjects.count());
}

QHash<int, QByteArray> GameObjects::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[SizeRole] = "size";
    roles[LayerRole] = "layer";
    roles[PositionRole] = "position";
    roles[CenterPositionRole] = "centerPosition";
    roles[ImageNameRole] = "imageName";
    roles[ResourcePathRole] = "resourcePath";
    roles[ActiveRole] = "activeRole";
    return roles;
}
