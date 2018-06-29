#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QPoint>

class Player : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)

    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(bool moving READ moving NOTIFY movingChanged)
    Q_PROPERTY(int size READ size WRITE setSize NOTIFY sizeChanged)

public:
    explicit Player(QObject *parent = nullptr);

    QString name() const;
    void setName(const QString &name);

    qreal x() const;
    qreal y() const;

    QPointF position() const;
    void setPosition(const QPointF position);

    int size() const;
    void setSize(const int &size);

    qreal angle() const;
    void setAngle(const qreal &angle);

    qreal speed() const;
    void setSpeed(const qreal speed);

    bool movable() const;
    void setMovable(const bool &movable);

    bool moving() const;
    void setMoving(bool moving);


private:
    QString m_name;
    QPointF m_position;
    qreal m_angle = 0;
    qreal m_speed = 0.4;
    bool m_movable = true;
    bool m_moving = false;
    int m_size;

signals:
    void nameChanged(const QString &name);
    void positionChanged(const QPointF &position);
    void sizeChanged(int size);
    void angleChanged(const qreal &angle);
    void speedChanged(const qreal &speed);
    void movableChanged(bool movable);
    void movingChanged(bool moving);

};

#endif // PLAYER_H
