#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QSize>
#include <QPoint>
#include <QPoint>
#include <QObject>

class GameObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF position READ position NOTIFY positionChanged)
    Q_PROPERTY(QPointF centerPosition READ centerPosition NOTIFY positionChanged)
    Q_PROPERTY(qreal layer READ layer NOTIFY layerChanged)
    Q_PROPERTY(Shape shape READ shape NOTIFY shapeChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)

public:
    enum Shape {
        ShapePoint,
        ShapeRectangle,
        ShapeCircle
    };
    Q_ENUM(Shape)

    explicit GameObject(QObject *parent = nullptr);
    virtual ~GameObject() = default;

    QString name() const;
    void setName(const QString &name);

    QPointF position() const;
    void setPosition(const QPointF &position);

    qreal layer() const;
    void setLayer(qreal layer);

    QPointF centerPosition() const;

    QSize size() const;
    void setSize(const QSize &size);

    Shape shape() const;
    void setShape(Shape shape);

private:
    QString m_name;
    QPointF m_position;
    qreal m_layer = 0;
    QSize m_size = QSize(1, 1);
    Shape m_shape = ShapeRectangle;

signals:
    void nameChanged(const QString &name);
    void positionChanged(const QPointF &position);
    void layerChanged(qreal layer);
    void sizeChanged(const QSize &size);
    void shapeChanged(Shape shape);

};

QDebug operator<<(QDebug debug, GameObject *gameObject);

#endif // GAMEOBJECT_H
