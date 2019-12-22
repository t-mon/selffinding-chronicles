#ifndef GAMEOBJECTS_H
#define GAMEOBJECTS_H

#include <QList>
#include <QObject>
#include <QAbstractListModel>

#include "gameobject.h"

class GameObjects : public QAbstractListModel
{
    Q_OBJECT
public:
    enum ObjectRole {
        NameRole = Qt::DisplayRole,
        SizeRole,
        PositionRole,
        CenterPositionRole,
        LayerRole,
        ImageNameRole,
        ResourcePathRole,
        ActiveRole
    };
    Q_ENUM(ObjectRole)

    explicit GameObjects(QObject *parent = nullptr);

    QList<GameObject *> gameObjects() const;
    Q_INVOKABLE GameObject *get(int index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void addGameObject(GameObject *object);
    void addGameObjectList(QList<GameObject *> objectList);
    void removeGameObject(GameObject *object);

    void clearModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<GameObject *> m_gameObjects;

signals:
    void countChanged(int count);

};

#endif // GAMEOBJECTS_H
