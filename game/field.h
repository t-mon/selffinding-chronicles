#ifndef FIELD_H
#define FIELD_H

#include <QPoint>
#include <QDebug>
#include <QImage>
#include <QObject>

class Map;
class GameItem;
class GameItems;
class GameObject;

class Field : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(bool playerOnField READ playerOnField NOTIFY playerOnFieldChanged)
    Q_PROPERTY(bool hasItem READ hasItem NOTIFY hasItemChanged)
    Q_PROPERTY(GameItems *gameItems READ gameItems CONSTANT)

    friend class Map;

public:
    explicit Field(QPoint position, QObject *parent = nullptr);

    QPoint position() const;

    bool playerOnField() const;
    void setPlayerOnField(bool playerOnField);

    bool hasItem() const;

    GameItems *gameItems() const;
    GameObject *collitionObject() const;

    Field *northField() const;
    Field *northEastField() const;
    Field *eastField() const;
    Field *southEastField() const;
    Field *southField() const;
    Field *southWestField() const;
    Field *westField() const;
    Field *northWestField() const;

    QList<Field *> getSurroundingFields() const;

private:
    QPoint m_position;
    bool m_accessible = true;
    bool m_playerOnField = false;
    bool m_inPlayerRange = false;

    GameItems *m_gameItems = nullptr;
    GameObject *m_collitionObject = nullptr;

    // Neighbors
    Field *m_northField = nullptr;
    Field *m_northEastField = nullptr;
    Field *m_eastField = nullptr;
    Field *m_southEastField = nullptr;
    Field *m_southField = nullptr;
    Field *m_southWestField = nullptr;
    Field *m_westField = nullptr;
    Field *m_northWestField = nullptr;

    void setNorthField(Field *field);
    void setNorthEastField(Field *field);
    void setEastField(Field *field);
    void setSouthEastField(Field *field);
    void setSouthField(Field *field);
    void setSouthWestField(Field *field);
    void setWestField(Field *field);
    void setNorthWestField(Field *field);

private slots:
    void onGameItemsCountChanged(int count);

signals:
    void positionChanged(const QPoint &position);
    void playerOnFieldChanged(bool playerOnField);
    void hasItemChanged(bool hasGameItem);
};

QDebug operator<<(QDebug debug, Field *field);

#endif // FIELD_H
