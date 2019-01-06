#include "gameitemsproxy.h"
#include "../debugcategories.h"

GameItemsProxy::GameItemsProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    connect(this, &GameItemsProxy::rowsInserted, this, &GameItemsProxy::onRowsInseted);
    connect(this, &GameItemsProxy::rowsRemoved, this, &GameItemsProxy::onRowsRemoved);
}

GameItems *GameItemsProxy::gameItems() const
{
    return m_gameItems;
}

void GameItemsProxy::setGameItems(GameItems *gameItems)
{
    if (m_gameItems == gameItems)
        return;

    // Disconnect old model
    if (m_gameItems) {
        disconnect(m_gameItems, &GameItems::countChanged, this, &GameItemsProxy::onSourceModelCountChanged);
    }

    m_gameItems = gameItems;
    emit gameItemsChanged(gameItems);

    setSourceModel(m_gameItems);

    if (m_gameItems) {
        connect(m_gameItems, &GameItems::countChanged, this, &GameItemsProxy::onSourceModelCountChanged);
        m_shownItemIds.clear();
        invalidateFilter();
        sort(0);
    }



    emit countChanged();
}

int GameItemsProxy::count() const
{
    return rowCount(QModelIndex());
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
    //qCDebug(dcGame()) << "Set filter item type" << m_itemTypeFilter;

    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
}

GameItem *GameItemsProxy::get(int index) const
{
    if (!m_gameItems)
        return nullptr;

    return m_gameItems->get(mapToSource(this->index(index, 0)).row());
}

void GameItemsProxy::resetFilter()
{
    m_shownItemIds.clear();
    setItemIdFilter(QString());
    setFilterDuplicates(false);
    setItemTypeFilter(GameItem::TypeNone);
}

QString GameItemsProxy::itemIdFilter() const
{
    return m_itemIdFilter;
}

void GameItemsProxy::setItemIdFilter(const QString &itemIdFilter)
{
    if (m_itemIdFilter == itemIdFilter)
        return;

    m_itemIdFilter = itemIdFilter;
    emit itemIdFilterChanged(m_itemIdFilter);
    //qCDebug(dcGame()) << "Set filter item id" << m_itemIdFilter;

    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
}

bool GameItemsProxy::filterDuplicates() const
{
    return m_filterDuplicates;
}

void GameItemsProxy::setFilterDuplicates(bool filterDuplicates)
{
    if (m_filterDuplicates == filterDuplicates)
        return;

    m_filterDuplicates = filterDuplicates;
    emit filterDuplicatesChanged(m_filterDuplicates);
    //qCDebug(dcGame()) << "Set filter duplicates" << m_filterDuplicates;
    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
}

bool GameItemsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    if (!m_gameItems)
        return true;

    GameItem *gameItem = m_gameItems->get(sourceRow);

    // If no filter, return always true
    if (m_itemTypeFilter == GameItem::TypeNone && !m_filterDuplicates && m_itemIdFilter.isEmpty())
        return true;

    // Filter out items not matching the given id filter
    if (!m_itemIdFilter.isEmpty() && gameItem->itemId() != m_itemIdFilter) {
        //qCDebug(dcGame()) << gameItem->name() << gameItem->itemId() << "filtered out because of the id filter" << m_itemIdFilter;
        return false;
    }

    // Filter out items not matching the filter
    if (m_itemTypeFilter != GameItem::TypeNone && gameItem->itemType() != m_itemTypeFilter) {
        //qCDebug(dcGame()) << gameItem->name() << gameItem->itemId() << "filtered out because of the type filter" << m_itemTypeFilter;
        return false;
    }

    // Filter out duplicates
    if (m_filterDuplicates) {
        if (m_shownItemIds.contains(gameItem->itemId())) {
            //qCDebug(dcGame()) << gameItem->name() << gameItem->itemId() << "already in the list";
            return false;
        } else {
            const_cast<GameItemsProxy *>(this)->m_shownItemIds.append(gameItem->itemId());
            //qCDebug(dcGame()) << gameItem->name() << gameItem->itemId() << "appended for duplicates filter";
        }
    }

    return true;
}

bool GameItemsProxy::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    // Alphabetically for now
    QVariant leftName = sourceModel()->data(left);
    QVariant rightName = sourceModel()->data(right);

    return QString::localeAwareCompare(leftName.toString(), rightName.toString()) < 0;
}

void GameItemsProxy::onSourceModelCountChanged()
{
    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
}

void GameItemsProxy::onRowsInseted(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    emit countChanged();
}

void GameItemsProxy::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    emit countChanged();
}
