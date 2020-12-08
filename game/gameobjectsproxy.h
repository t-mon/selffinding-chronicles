#ifndef GAMEOBJECTSPROXY_H
#define GAMEOBJECTSPROXY_H

#include <QRectF>
#include <QObject>
#include <QSortFilterProxyModel>

#include "gameobjects.h"

class GameObjectsProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(QRectF viewFilter READ viewFilter WRITE setViewFilter NOTIFY viewFilterChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)

public:
    explicit GameObjectsProxy(QObject *parent = nullptr);

    GameObjects *gameObjects() const;
    void setGameObjects(GameObjects *gameObjects);

    int count() const;

    QRectF viewFilter() const;
    void setViewFilter(const QRectF &viewFilter);

    Q_INVOKABLE GameObject *get(int index) const;
    Q_INVOKABLE void resetFilter();

private:
    GameObjects *m_gameObjects = nullptr;

    QRectF m_viewFilter = QRectF();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

signals:
    void gameObjectsChanged(GameObjects *gameObjects);
    void countChanged();

    void viewFilterChanged(const QRectF &viewFilter);
    void gameObjectActiveChanged(GameObject *object, bool active);

private slots:
    void onSourceModelCountChanged();
    void onRowsInseted(const QModelIndex &parent, int first, int last);
    void onRowsRemoved(const QModelIndex &parent, int first, int last);
    void onRowsAboutToBeRemoved(const QModelIndex &parent, int first, int last);

};

#endif // GAMEOBJECTSPROXY_H
