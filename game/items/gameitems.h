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
    };
    Q_ENUM(ItemRole)

    explicit GameItems(QObject *parent = nullptr);

    QList<GameItem *> gameItems() const;
    Q_INVOKABLE GameItem *get(int index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void addGameItem(GameItem *item);
    void removeGameItem(GameItem *item);

    void clearModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<GameItem *> m_gameItems;

};

#endif // GAMEITEMS_H
