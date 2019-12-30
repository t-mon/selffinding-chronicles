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

GameItem *GameItems::getFirstGameItem(const QString &itemId)
{
    foreach (GameItem *gameItem, m_gameItems) {
        if (gameItem->itemId() == itemId) {
            return gameItem;
        }
    }

    return nullptr;
}

GameItem *GameItems::getLastGameItem(const QString &itemId)
{
    QListIterator<GameItem *> iterator(m_gameItems);
    iterator.toBack();
    while (iterator.hasPrevious()) {
        GameItem *item = iterator.previous();
        if (item->itemId() == itemId) {
            return item;
        }
    }

    return nullptr;
}

GameItem *GameItems::takeLastGameItem(const QString &itemId)
{
    GameItem *item = getLastGameItem(itemId);
    if (item) removeGameItem(item);
    return item;
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
    } else if (role == ItemIdRole) {
        return gameItem->itemId();
    } else if (role == PriceRole) {
        return gameItem->price();
    }

    return QVariant();
}

void GameItems::setAutoParent(bool autoParent)
{
    m_autoParent = autoParent;
}

void GameItems::addGameItem(GameItem *item)
{
    if (m_gameItems.contains(item))
        return;

    if (m_autoParent)
        item->setParent(this);

    beginInsertRows(QModelIndex(), m_gameItems.count(), m_gameItems.count());
    m_gameItems.append(item);
    endInsertRows();

    emit countChanged(m_gameItems.count());
}

void GameItems::addGameItemList(QList<GameItem *> itemList)
{
    foreach (GameItem *item, itemList) {
        addGameItem(item);
    }
}

void GameItems::removeGameItem(GameItem *item)
{
    if (!m_gameItems.contains(item))
        return;

    int index = m_gameItems.indexOf(item);
    beginRemoveRows(QModelIndex(), index, index);
    m_gameItems.removeAt(index);
    endRemoveRows();

    emit countChanged(m_gameItems.count());
}

void GameItems::clearModel()
{
    beginResetModel();
    foreach (GameItem *item, m_gameItems) {
        item->deleteLater();
    }
    m_gameItems.clear();
    endResetModel();

    emit countChanged(m_gameItems.count());
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
    roles[ItemIdRole] = "itemId";
    roles[LayerRole] = "layer";
    roles[PriceRole] = "price";
    return roles;
}
