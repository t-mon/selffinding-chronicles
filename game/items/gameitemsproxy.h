#ifndef GAMEITEMSPROXY_H
#define GAMEITEMSPROXY_H

#include <QObject>
#include <QSortFilterProxyModel>

#include "gameitems.h"

class GameItemsProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(GameItem::Type itemTypeFilter READ itemTypeFilter WRITE setItemTypeFilter NOTIFY itemTypeFilterChanged)

public:
    explicit GameItemsProxy(QObject *parent = nullptr);

    GameItems *gameItems() const;
    void setGameItems(GameItems *gameItems);

    GameItem::Type itemTypeFilter() const;
    void setItemTypeFilter(GameItem::Type type);

    Q_INVOKABLE GameItem *get(int index) const;
    Q_INVOKABLE void resetFilter();

private:
    GameItems *m_gameItems = nullptr;
    GameItem::Type m_itemTypeFilter = GameItem::TypeNone;

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

signals:
    void gameItemsChanged(GameItems *gameItems);
    void itemTypeFilterChanged(GameItem::Type itemTypeFilter);

};

#endif // GAMEITEMSPROXY_H
