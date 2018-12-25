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
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
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


    enum CategoryFlag {CategoryCharacter = 0x0001,
                       CategoryObstacle = 0x0002,
                       CategoryFriend = 0x0004,
                       CategoryEnemey = 0x0008,
                       CategoryBullet = 0x0010,
                       Category = 0x0020, Category7 = 0x0040, Category8 = 0x0080,
                       Category9 = 0x0100, Category10 = 0x0200, Category11 = 0x0400, Category12 = 0x0800,
                       Category13 = 0x1000, Category14 = 0x2000, Category15 = 0x4000, Category16 = 0x8000,
                       All = 0xFFFF, None=0x0000};

    Q_DECLARE_FLAGS(CategoryFlags, CategoryFlag)

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
