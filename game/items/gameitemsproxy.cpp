#include "gameitemsproxy.h"

GameItemsProxy::GameItemsProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{

}

GameItems *GameItemsProxy::gameItems() const
{
    return m_gameItems;
}

void GameItemsProxy::setGameItems(GameItems *gameItems)
{
    if (m_gameItems == gameItems)
        return;

    m_gameItems = gameItems;
    emit gameItemsChanged(gameItems);
    setSourceModel(m_gameItems);
    invalidateFilter();
    sort(0);
}

GameItem::Type GameItemsProxy::itemTypeFilter() const
{
    return m_itemTypeFilter;
}

void GameItemsProxy::setItemTypeFilter(GameItem::Type type)
{
    if (m_itemTypeFilter == type)
        return;

    m_itemTypeFilter = type;
    emit itemTypeFilterChanged(m_itemTypeFilter);

    invalidateFilter();
    sort(0);
}

GameItem *GameItemsProxy::get(int index) const
{
    return m_gameItems->get(mapToSource(this->index(index, 0)).row());
}

void GameItemsProxy::resetFilter()
{
    setItemTypeFilter(GameItem::TypeNone);
}

bool GameItemsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    GameItem *gameItem = m_gameItems->get(sourceRow);

    // If no filter, return always true
    if (m_itemTypeFilter == GameItem::TypeNone)
        return true;

    // Filter out items not matching the filter
    if (gameItem->itemType() != m_itemTypeFilter)
        return false;

    return true;
}

bool GameItemsProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftName = sourceModel()->data(left);
    QVariant rightName = sourceModel()->data(right);

    return QString::localeAwareCompare(leftName.toString(), rightName.toString()) < 0;
}
