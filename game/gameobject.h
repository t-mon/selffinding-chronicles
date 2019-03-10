#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QSize>
#include <QPoint>
#include <QPoint>
#include <QVector>
#include <QObject>
#include <QQmlListProperty>

class GameObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QPointF centerPosition READ centerPosition NOTIFY positionChanged)
    Q_PROPERTY(double layer READ layer NOTIFY layerChanged)
    Q_PROPERTY(QSize size READ size NOTIFY sizeChanged)

    Q_PROPERTY(Shape shape READ shape NOTIFY shapeChanged)
    Q_PROPERTY(BodyType bodyType READ bodyType NOTIFY bodyTypeChanged)
    Q_PROPERTY(QSize physicsSize READ physicsSize NOTIFY physicsSizeChanged)
    Q_PROPERTY(QPointF physicsPosition READ physicsPosition NOTIFY physicsPositionChanged)
    Q_PROPERTY(PhysicsFlags categoryFlag READ categoryFlag NOTIFY categoryFlagChanged)
    Q_PROPERTY(PhysicsFlags collisionFlag READ collisionFlag NOTIFY collisionFlagChanged)
    Q_PROPERTY(QQmlListProperty<QPointF> vertices READ vertices NOTIFY verticesChanged)

    Q_FLAGS(PhysicsFlags)

public:
    enum Shape {
        ShapeNone,
        ShapePoint,
        ShapeRectangle,
        ShapeCircle,
        ShapePolygon
    };
    Q_ENUM(Shape)

    enum BodyType {
        BodyTypeStatic = 0,
        BodyTypeKinematic,
        BodyTypeDynamic
    };
    Q_ENUM(BodyType)

    enum PhysicsFlag {
        PhysicsNone         = 0x0000,
        PhysicsCharacter    = 0x0001,
        PhysicsEnemy        = 0x0002,
        PhysicsPickableItem = 0x0004,
        PhysicsStaticItem   = 0x0008,
        PhysicsWalkableItem = 0x0010,
        PhysicsSensor       = 0x0020,
        PhysicsWeapon       = 0x0040,
        PhysicsBullet       = 0x0080,
        PhysicsMagic        = 0x0100,
        PhysicsCategory10   = 0x0200,
        PhysicsCategory11   = 0x0400,
        PhysicsCategory12   = 0x0800,
        PhysicsCategory13   = 0x1000,
        PhysicsCategory14   = 0x2000,
        PhysicsCategory15   = 0x4000,
        PhysicsCategory16   = 0x8000,
        PhysicsAll          = 0xFFFF
    };
    Q_ENUM(PhysicsFlag)
    Q_DECLARE_FLAGS(PhysicsFlags, PhysicsFlag)

    explicit GameObject(QObject *parent = nullptr);
    virtual ~GameObject() = default;

    QString name() const;
    void setName(const QString &name);

    QPointF position() const;
    void setPosition(const QPointF &position);

    double layer() const;
    void setLayer(double layer);

    QPointF centerPosition() const;

    QSize size() const;
    void setSize(const QSize &size);

    // Physics
    Shape shape() const;
    void setShape(Shape shape);

    BodyType bodyType() const;
    void setBodyType(BodyType bodyType);

    QSize physicsSize() const;
    void setPhysicsSize(const QSize &physicsSize);

    QPointF physicsPosition() const;
    void setPhysicsPosition(const QPointF &physicsPosition);

    PhysicsFlags categoryFlag() const;
    void setCategoryFlag(PhysicsFlags categoryFlag);

    PhysicsFlags collisionFlag() const;
    void setCollisionFlag(PhysicsFlags collisionFlag);

    QQmlListProperty<QPointF> vertices() const;
    void setVertices(const QQmlListProperty<QPointF> &vertices);

private:
    QString m_name;
    QPointF m_position;
    double m_layer = 0;
    QSize m_size = QSize(1, 1);

    Shape m_shape = ShapeNone;
    BodyType m_bodyType = BodyTypeStatic;
    QSize m_physicsSize = QSize(0, 0);
    QPointF m_physicsPosition;
    PhysicsFlags m_categoryFlag = PhysicsNone;
    PhysicsFlags m_collisionFlag = PhysicsNone;
    QQmlListProperty<QPointF> m_vertices;

signals:
    void nameChanged(const QString &name);
    void positionChanged(const QPointF &position);
    void layerChanged(double layer);
    void sizeChanged(const QSize &size);

    void shapeChanged(Shape shape);
    void bodyTypeChanged(BodyType bodyType);
    void physicsSizeChanged(const QSize &physicsSize);
    void physicsPositionChanged(const QPointF &physicsPosition);
    void categoryFlagChanged(PhysicsFlags categoryFlag);
    void collisionFlagChanged(PhysicsFlags collisionFlag);
    void verticesChanged(const QQmlListProperty<QPointF> &vertices);

};

QDebug operator<<(QDebug debug, GameObject *gameObject);

#endif // GAMEOBJECT_H
