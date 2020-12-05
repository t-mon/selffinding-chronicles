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
    beginResetModel();
    if (m_gameItems)
        disconnect(m_gameItems, &GameItems::countChanged, this, &GameItemsProxy::onSourceModelCountChanged);

    qCDebug(dcGameModels()) << "Set items" << this << gameItems;
    m_gameItems = gameItems;
    setSourceModel(m_gameItems);
    setSortRole(GameItems::PriceRole);
    emit gameItemsChanged(m_gameItems);

    if (m_gameItems) {
        connect(m_gameItems, &GameItems::countChanged, this, &GameItemsProxy::onSourceModelCountChanged);
    }

    endResetModel();

    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);

    emit countChanged();
}

int GameItemsProxy::count() const
{
    return rowCount(QModelIndex());
}

GameItem *GameItemsProxy::get(int index) const
{
    if (!m_gameItems)
        return nullptr;

    return m_gameItems->get(mapToSource(this->index(index, 0)).row());
}

void GameItemsProxy::resetFilter()
{
    qCDebug(dcGameModels()) << "Reset filters" << this;

    m_shownItemIds.clear();
    setItemIdFilter(QString());
    setFilterDuplicates(false);
    setViewFilter(QRectF());
    setItemTypeFilter(GameItem::TypeNone);
}

QRectF GameItemsProxy::viewFilter() const
{
    return m_viewFilter;
}

void GameItemsProxy::setViewFilter(const QRectF &viewFilter)
{
    if (m_viewFilter == viewFilter)
        return;

    qCDebug(dcGameModels()) << "Set filter viewfilter" << viewFilter << this;
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

    qCDebug(dcGameModels()) << "Set filter item type" << type << this;
    m_itemTypeFilter = type;
    emit itemTypeFilterChanged(m_itemTypeFilter);
    //qCDebug(dcGame()) << "Set filter item type" << m_itemTypeFilter;

    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
    emit countChanged();
}

QString GameItemsProxy::itemIdFilter() const
{
    return m_itemIdFilter;
}

void GameItemsProxy::setItemIdFilter(const QString &itemIdFilter)
{
    if (m_itemIdFilter == itemIdFilter)
        return;

    qCDebug(dcGameModels()) << "Set filter item id" << itemIdFilter << this;
    m_itemIdFilter = itemIdFilter;
    emit itemIdFilterChanged(m_itemIdFilter);
    //qCDebug(dcGame()) << "Set filter item id" << m_itemIdFilter;

    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
    emit countChanged();
}

bool GameItemsProxy::filterDuplicates() const
{
    return m_filterDuplicates;
}

void GameItemsProxy::setFilterDuplicates(bool filterDuplicates)
{
    if (m_filterDuplicates == filterDuplicates)
        return;

    qCDebug(dcGameModels()) << "Set filter duplicates" << filterDuplicates << this;
    m_filterDuplicates = filterDuplicates;
    emit filterDuplicatesChanged(m_filterDuplicates);
    //qCDebug(dcGame()) << "Set filter duplicates" << m_filterDuplicates;
    m_shownItemIds.clear();
    invalidateFilter();
    sort(0);
    emit countChanged();
}

bool GameItemsProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    // Get the current game item
    if (!m_gameItems)
        return false;

    GameItem *gameItem = m_gameItems->get(sourceRow);
    if (!gameItem)
        return false;

    qCDebug(dcGameModels()) << "--> Check filter for" << gameItem;

    // If no filter, always keep item in model
    if (m_itemTypeFilter == GameItem::TypeNone && !m_filterDuplicates && m_itemIdFilter.isEmpty() && !m_viewFilter.isValid()) {
        qCDebug(dcGameModels()) << this << "No filter" << gameItem << true;
        return true;
    }

    // Filter out items not matching the given id filter
    if (!m_itemIdFilter.isEmpty() && gameItem->itemId() != m_itemIdFilter) {
        qCDebug(dcGameModels()) << this << "Id filter" << m_itemIdFilter << gameItem << false;
        return false;
    }

    // Filter out items not in the view filter
    if (m_viewFilter.isValid() && !m_viewFilter.intersects(QRectF(gameItem->position(), gameItem->size()))) {
        qCDebug(dcGameModels()) << this << "View filter" << m_viewFilter << gameItem << true;
        return false;
    }

    // Filter out items not matching the item type filter
    if (m_itemTypeFilter != GameItem::TypeNone && gameItem->itemType() != m_itemTypeFilter) {
        qCDebug(dcGameModels()) << this << "Type filter" << m_itemTypeFilter << gameItem << true;
        return false;
    }

    // Filter out duplicates
    if (m_filterDuplicates) {
        if (m_shownItemIds.contains(gameItem->itemId())) {
            qCDebug(dcGameModels()) << this << "Filter duplicates" << gameItem << false;
            return false;
        } else {
            //const_cast<GameItemsProxy *>(this)->m_shownItemIds.append(gameItem->itemId());
            const_cast<GameItemsProxy *>(this)->m_shownItemIds.append(gameItem->itemId());
            qCDebug(dcGameModels()) << this << "Filter duplicates" << gameItem << true;
            return true;
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
    emit countChanged();
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
