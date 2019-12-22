#ifndef GAMEITEMSPROXY_H
#define GAMEITEMSPROXY_H

#include <QRectF>
#include <QObject>
#include <QStringList>
#include <QSortFilterProxyModel>

#include "gameitems.h"

class GameItemsProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(GameItems *gameItems READ gameItems WRITE setGameItems NOTIFY gameItemsChanged)

    Q_PROPERTY(QRectF viewFilter READ viewFilter WRITE setViewFilter NOTIFY viewFilterChanged)
    Q_PROPERTY(QString itemIdFilter READ itemIdFilter WRITE setItemIdFilter NOTIFY itemIdFilterChanged)
    Q_PROPERTY(GameItem::Type itemTypeFilter READ itemTypeFilter WRITE setItemTypeFilter NOTIFY itemTypeFilterChanged)
    Q_PROPERTY(bool filterDuplicates READ filterDuplicates WRITE setFilterDuplicates NOTIFY filterDuplicatesChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit GameItemsProxy(QObject *parent = nullptr);

    GameItems *gameItems() const;
    void setGameItems(GameItems *gameItems);

    int count() const;

    Q_INVOKABLE GameItem *get(int index) const;
    Q_INVOKABLE void resetFilter();

    QRectF viewFilter() const;
    void setViewFilter(const QRectF &viewFilter);

    GameItem::Type itemTypeFilter() const;
    void setItemTypeFilter(GameItem::Type type);

    QString itemIdFilter() const;
    void setItemIdFilter(const QString &itemIdFilter);

    bool filterDuplicates() const;
    void setFilterDuplicates(bool filterDuplicates);

private:
    GameItems *m_gameItems = nullptr;

    QRectF m_viewFilter;
    QStringList m_shownItemIds;
    QString m_itemIdFilter;
    GameItem::Type m_itemTypeFilter = GameItem::TypeNone;
    bool m_filterDuplicates = false;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

signals:
    void gameItemsChanged(GameItems *gameItems);
    void countChanged();

    void viewFilterChanged(const QRectF &viewFilter);
    void itemIdFilterChanged(const QString &itemIdFilter);
    void itemTypeFilterChanged(GameItem::Type itemTypeFilter);
    void filterDuplicatesChanged(bool filterDuplicates);

    void gameItemActiveChanged(GameItem *item, bool active);

private slots:
    void onSourceModelCountChanged();
    void onRowsInseted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

};

#endif // GAMEITEMSPROXY_H
