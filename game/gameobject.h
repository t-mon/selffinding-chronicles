#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QSizeF>
#include <QPointF>
#include <QObject>
#include <QQmlListProperty>

class GameObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString resourcePath READ resourcePath CONSTANT)
    Q_PROPERTY(QString itemId READ itemId CONSTANT)
    Q_PROPERTY(QString imageName READ imageName NOTIFY imageNameChanged)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QPointF centerPosition READ centerPosition NOTIFY positionChanged)
    Q_PROPERTY(double layer READ layer NOTIFY layerChanged)
    Q_PROPERTY(QSizeF size READ size WRITE setSize NOTIFY sizeChanged)
    Q_PROPERTY(bool focusVisible READ focusVisible NOTIFY focusVisibleChanged)
    Q_PROPERTY(bool backgroundObject READ backgroundObject CONSTANT)
    Q_PROPERTY(bool active READ active NOTIFY activeChanged)

    Q_PROPERTY(Shape shape READ shape NOTIFY shapeChanged)
    Q_PROPERTY(BodyType bodyType READ bodyType NOTIFY bodyTypeChanged)
    Q_PROPERTY(QSizeF physicsSize READ physicsSize NOTIFY physicsSizeChanged)
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
        PhysicsBodyHitbox   = 0x0200,
        PhysicsFire         = 0x0400,
        PhysicsCategory12   = 0x0800,
        PhysicsCategory13   = 0x1000,
        PhysicsCategory14   = 0x2000,
        PhysicsCategory15   = 0x4000,
        PhysicsCategory16   = 0x8000,
        PhysicsAll          = 0xFFFF
    };
    Q_ENUM(PhysicsFlag)
    Q_DECLARE_FLAGS(PhysicsFlags, PhysicsFlag)

    enum Layer {
        LayerBackground, // -2
        LayerBase, // -1
        LayerItems, // y = z
        LayerOverlay // height + 1
    };
    Q_ENUM(Layer)

    explicit GameObject(QObject *parent = nullptr);
    virtual ~GameObject() = default;

    QString itemId() const;
    void setItemId(const QString &itemId);

    QString resourcePath() const;
    void setResourcePath(const QString &resourcePath);

    QString imageName() const;
    void setImageName(const QString &imageName);

    QString name() const;
    void setName(const QString &name);

    QPointF position() const;
    void setPosition(const QPointF &position);

    double rotationAngle() const;
    void setRotationAngle(double rotationAngle);

    Layer layer() const;
    void setLayer(Layer layer);

    QPointF centerPosition() const;

    QSizeF size() const;
    void setSize(const QSizeF &size);

    bool focusVisible() const;
    void setFocusVisible(bool focusVisible);

    bool backgroundObject() const;
    void setBackgroundObject(bool backgroundObject);

    bool active() const;
    void setActive(bool active);

    // Physics
    Shape shape() const;
    void setShape(Shape shape);

    BodyType bodyType() const;
    void setBodyType(BodyType bodyType);

    QSizeF physicsSize() const;
    void setPhysicsSize(const QSizeF &physicsSize);

    QPointF physicsPosition() const;
    void setPhysicsPosition(const QPointF &physicsPosition);

    PhysicsFlags categoryFlag() const;
    void setCategoryFlag(PhysicsFlags categoryFlag);

    PhysicsFlags collisionFlag() const;
    void setCollisionFlag(PhysicsFlags collisionFlag);

    QQmlListProperty<QPointF> vertices() const;
    void setVertices(const QQmlListProperty<QPointF> &vertices);

private:
    QString m_itemId;
    QString m_imageName = "/images/game/placeholder.png";
    QString m_resourcePath;
    QString m_name;
    QPointF m_position;
    double m_rotationAngle = 0;
    Layer m_layer = LayerBase;
    QSizeF m_size = QSizeF(1, 1);
    bool m_focusVisible = true;
    bool m_backgroundObject = false;
    bool m_active = false;

    Shape m_shape = ShapeNone;
    BodyType m_bodyType = BodyTypeStatic;
    QSizeF m_physicsSize = QSizeF(0, 0);
    QPointF m_physicsPosition;
    PhysicsFlags m_categoryFlag = PhysicsNone;
    PhysicsFlags m_collisionFlag = PhysicsNone;
    QQmlListProperty<QPointF> m_vertices;

signals:
    void imageNameChanged(const QString &imageName);
    void nameChanged(const QString &name);
    void positionChanged(const QPointF &position);
    void rotationAngleChanged(double rotationAngle);
    void layerChanged(Layer layer);
    void sizeChanged(const QSizeF &size);
    void focusVisibleChanged(bool focusVisible);
    void activeChanged(bool active);

    void shapeChanged(Shape shape);
    void bodyTypeChanged(BodyType bodyType);
    void physicsSizeChanged(const QSizeF &physicsSize);
    void physicsPositionChanged(const QPointF &physicsPosition);
    void categoryFlagChanged(PhysicsFlags categoryFlag);
    void collisionFlagChanged(PhysicsFlags collisionFlag);
    void verticesChanged(const QQmlListProperty<QPointF> &vertices);

};

QDebug operator<<(QDebug debug, GameObject *gameObject);

#endif // GAMEOBJECT_H
