#ifndef GAMEITEMS_H
#define GAMEITEMS_H

#include <QObject>
#include <QAbstractListModel>

#include "gameitem.h"

class GameItems : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ItemRole {
        NameRole = Qt::DisplayRole,
        PositionRole,
        LayerRole,
        CenterPositionRole,
        SizeRole,
        InteractionRole,
        ImageNameRole,
        ItemTypeRole,
        ItemIdRole,
        PriceRole
    };
    Q_ENUM(ItemRole)

    explicit GameItems(QObject *parent = nullptr);

    QList<GameItem *> gameItems() const;
    Q_INVOKABLE GameItem *get(int index);

    GameItem *getFirstGameItem(const QString &itemId);
    GameItem *getLastGameItem(const QString &itemId);
    GameItem *takeLastGameItem(const QString &itemId);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void setAutoParent(bool autoParent);

    void addGameItem(GameItem *item);
    void addGameItemList(QList<GameItem *> itemList);
    void removeGameItem(GameItem *item);

    void clearModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<GameItem *> m_gameItems;
    bool m_autoParent = true;

signals:
    void countChanged(int count);

};

#endif // GAMEITEMS_H
