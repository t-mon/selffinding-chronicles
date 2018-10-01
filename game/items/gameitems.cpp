#include "gameitems.h"

GameItems::GameItems(QObject *parent) : QAbstractListModel(parent)
{

}

QList<GameItem *> GameItems::gameItems() const
{
    return m_gameItems;
}

GameItem *GameItems::get(int index)
{
    if (index >= m_gameItems.count() || index < 0)
        return nullptr;

    return m_gameItems.at(index);
}

int GameItems::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_gameItems.count();
}

QVariant GameItems::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_gameItems.count())
        return QVariant();

    GameItem *gameItem = m_gameItems.at(index.row());
    if (!gameItem)
        return QVariant();

    if (role == PositionRole) {
        return gameItem->position();
    } else if (role == NameRole) {
        return gameItem->name();
    } else if (role == CenterPositionRole) {
        return gameItem->centerPosition();
    } else if (role == SizeRole) {
        return gameItem->size();
    } else if (role == InteractionRole) {
        return gameItem->interaction();
    } else if (role == ImageNameRole) {
        return gameItem->imageName();
    } else if (role == ItemTypeRole) {
        return gameItem->itemType();
    } else if (role == LayerRole) {
        return gameItem->layer();
    }

    return QVariant();
}

void GameItems::addGameItem(GameItem *item)
{
    if (m_gameItems.contains(item))
        return;

    beginInsertRows(QModelIndex(), m_gameItems.count(), m_gameItems.count());
    m_gameItems.append(item);
    endInsertRows();
}

void GameItems::removeGameItem(GameItem *item)
{
    if (!m_gameItems.contains(item))
        return;

    int index = m_gameItems.indexOf(item);
    beginRemoveRows(QModelIndex(), index, index);
    m_gameItems.removeOne(item);
    endRemoveRows();
}

void GameItems::clearModel()
{
    beginResetModel();
    m_gameItems.clear();
    endResetModel();
}

QHash<int, QByteArray> GameItems::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[PositionRole] = "position";
    roles[CenterPositionRole] = "centerPosition";
    roles[SizeRole] = "size";
    roles[InteractionRole] = "interaction";
    roles[ImageNameRole] = "imageName";
    roles[ItemTypeRole] = "itemType";
    roles[LayerRole] = "layer";
    return roles;
}
