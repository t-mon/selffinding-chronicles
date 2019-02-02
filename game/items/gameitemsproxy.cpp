#include "gameitemsproxy.h"
#include "../debugcategories.h"

GameItemsProxy::GameItemsProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    connect(this, &GameItemsProxy::rowsInserted, this, &GameItemsProxy::onRowsInseted);
    connect(this, &GameItemsProxy::rowsRemoved, this, &GameItemsProxy::onRowsRemoved);
    connect(this, &GameItemsProxy::rowsAboutToBeRemoved, this, &GameItemsProxy::onRowsAboutToBeRemoved);
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
    if (m_gameItems)
        disconnect(m_gameItems, &GameItems::countChanged, this, &GameItemsProxy::onSourceModelCountChanged);

    beginResetModel();
    m_gameItems = gameItems;
    emit gameItemsChanged(gameItems);
    setSourceModel(m_gameItems);
    setSortRole(GameItems::PriceRole);
    endResetModel();

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

QRectF GameItemsProxy::viewFilter() const
{
    return m_viewFilter;
}

void GameItemsProxy::setViewFilter(const QRectF &viewFilter)
{
    if (m_viewFilter == viewFilter)
        return;

    m_viewFilter = viewFilter;
    emit viewFilterChanged(m_viewFilter);

    invalidateFilter();
    emit countChanged();
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

    // Get the current game item
    if (!m_gameItems)
        return true;

    GameItem *gameItem = m_gameItems->get(sourceRow);
    if (!gameItem)
        return true;

    // If no filter, return always true
    if (m_itemTypeFilter == GameItem::TypeNone && !m_filterDuplicates && m_itemIdFilter.isEmpty() && m_viewFilter.isNull())
        return true;

    // Filter out items not matching the given id filter
    if (!m_itemIdFilter.isEmpty() && gameItem->itemId() != m_itemIdFilter)
        return false;

    // Filter out items not in the view filter
    if (!m_viewFilter.isNull() && !m_viewFilter.intersects(QRectF(gameItem->position(), gameItem->size())))
        return false;

    // Filter out items not matching the item type filter
    if (m_itemTypeFilter != GameItem::TypeNone && gameItem->itemType() != m_itemTypeFilter)
        return false;

    // Filter out duplicates
    if (m_filterDuplicates) {
        if (m_shownItemIds.contains(gameItem->itemId())) {
            return false;
        } else {
            const_cast<GameItemsProxy *>(this)->m_shownItemIds.append(gameItem->itemId());
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

    for (int i = first; i <= last; i++) {
        GameItem *item = get(i);
        if (item) {
            emit gameItemActiveChanged(item, true);
        }
    }

    emit countChanged();
}

void GameItemsProxy::onRowsRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)
    Q_UNUSED(first)
    Q_UNUSED(last)

    emit countChanged();
}

void GameItemsProxy::onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last)
{
    Q_UNUSED(parent)

    for (int i = first; i <= last; i++) {
        GameItem *item = get(i);
        if (item) {
            emit gameItemActiveChanged(item, false);
        }
    }

    emit countChanged();
}
