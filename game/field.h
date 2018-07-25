#ifndef FIELD_H
#define FIELD_H

#include <QPoint>
#include <QObject>
#include <QImage>

class Map;
class GameItem;

class Field : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(QString imageName READ imageName NOTIFY imageNameChanged)
    Q_PROPERTY(bool accessible READ accessible WRITE setAccessible NOTIFY accessibleChanged)
    Q_PROPERTY(bool playerOnField READ playerOnField NOTIFY playerOnFieldChanged)
    Q_PROPERTY(bool inPlayerRange READ inPlayerRange NOTIFY inPlayerRangeChanged)
    Q_PROPERTY(GameItem *gameItem READ gameItem NOTIFY gameItemChanged)

    friend class Map;

public:
    explicit Field(QPoint position, QObject *parent = nullptr);

    QPoint position() const;

    QString imageName() const;
    void setImageName(const QString &imageName);

    bool accessible() const;
    void setAccessible(bool accessible);

    bool playerOnField() const;
    void setPlayerOnField(bool playerOnField);

    bool inPlayerRange() const;
    void setInPlayerRange(bool inPlayerRange);

    GameItem *gameItem() const;
    void setGameItem(GameItem *gameItem);

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
    QString m_imageName;
    bool m_accessible = true;
    bool m_playerOnField = false;
    bool m_inPlayerRange = false;

    GameItem *m_gameItem = nullptr;

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

signals:
    void positionChanged(const QPoint &position);
    void imageNameChanged(const QString &imageName);
    void accessibleChanged(bool accessible);
    void playerOnFieldChanged(bool playerOnField);
    void inPlayerRangeChanged(bool inPlayerRange);
    void gameItemChanged(GameItem *gameItem);

};

#endif // FIELD_H
