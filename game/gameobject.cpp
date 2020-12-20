#include "gameobject.h"
#include "debugcategories.h"

GameObject::GameObject(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<PhysicsFlags>("PhysicsFlags");
}

QString GameObject::itemId() const
{
    return m_itemId;
}

void GameObject::setItemId(const QString &itemId)
{
    m_itemId = itemId;
}

QString GameObject::resourcePath() const
{
    return m_resourcePath;
}

void GameObject::setResourcePath(const QString &resourcePath)
{
    m_resourcePath = resourcePath;
}

QString GameObject::imageName() const
{
    return m_imageName;
}

void GameObject::setImageName(const QString &imageName)
{
    if (m_imageName == imageName)
        return;

    qCDebug(dcGameObject()) << this << "image name changed" << imageName;
    m_imageName = imageName;
    emit imageNameChanged(m_imageName);
}


QString GameObject::name() const
{
    return m_name;
}

void GameObject::setName(const QString &name)
{
    if (m_name == name)
        return;

    qCDebug(dcGameObject()) << "Name changed" << name;
    m_name = name;
    emit nameChanged(m_name);
}

QPointF GameObject::position() const
{
    return m_position;
}

void GameObject::setPosition(const QPointF &position)
{
    if (m_position == position)
        return;

    qCDebug(dcGameObject()) << this << "Position changed" << position;
    m_position = position;
    emit positionChanged(m_position);
}

double GameObject::rotationAngle() const
{
    return m_rotationAngle;
}

void GameObject::setRotationAngle(double rotationAngle)
{
    if (m_rotationAngle == rotationAngle)
        return;

    qCDebug(dcGameObject()) << this << "Rotation angle changed" << rotationAngle;
    m_rotationAngle = rotationAngle;
    emit rotationAngleChanged(m_rotationAngle);
}

GameObject::Layer GameObject::layer() const
{
    return m_layer;
}

void GameObject::setLayer(Layer layer)
{
    m_layer = layer;
    emit layerChanged(m_layer);
}

QPointF GameObject::centerPosition() const
{
    return m_position + QPointF(m_size.width() / 2.0, m_size.height() / 2.0);
}

QSizeF GameObject::size() const
{
    return m_size;
}

void GameObject::setSize(const QSizeF &size)
{
    if (m_size == size)
        return;

    qCDebug(dcGameObject()) << this << "Size changed" << size;
    m_size = size;
    emit sizeChanged(m_size);
}

bool GameObject::focusVisible() const
{
    return m_focusVisible;
}

void GameObject::setFocusVisible(bool focusVisible)
{
    if (m_focusVisible == focusVisible)
        return;

    qCDebug(dcGameObject()) << this << "Focus visible changed" << focusVisible;
    m_focusVisible = focusVisible;
    emit focusVisibleChanged(m_focusVisible);
}

bool GameObject::backgroundObject() const
{
    return m_backgroundObject;
}

void GameObject::setBackgroundObject(bool backgroundObject)
{
    if (m_backgroundObject == backgroundObject)
        return;

    m_backgroundObject = backgroundObject;
}

bool GameObject::active() const
{
    return m_active;
}

void GameObject::setActive(bool active)
{
    if (m_active == active)
        return;

    qCDebug(dcGameObject()) << this << "Active changed" << active;
    m_active = active;
    emit activeChanged(m_active);
}

GameObject::Shape GameObject::shape() const
{
    return m_shape;
}

void GameObject::setShape(GameObject::Shape shape)
{
    if (m_shape == shape)
        return;

    qCDebug(dcGameObject()) << "Shape changed" << shape;
    m_shape = shape;
    emit shapeChanged(m_shape);
}

GameObject::BodyType GameObject::bodyType() const
{
    return m_bodyType;
}

void GameObject::setBodyType(GameObject::BodyType bodyType)
{
    if (m_bodyType == bodyType)
        return;

    qCDebug(dcGameObject()) << this << "Body type changed" << bodyType;
    m_bodyType = bodyType;
    emit bodyTypeChanged(m_bodyType);
}

QSizeF GameObject::physicsSize() const
{
    return m_physicsSize;
}

void GameObject::setPhysicsSize(const QSizeF &physicsSize)
{
    if (m_physicsSize == physicsSize)
        return;

    qCDebug(dcGameObject()) << this << "Physics size changed" << physicsSize;
    m_physicsSize = physicsSize;
    emit physicsSizeChanged(m_physicsSize);
}

QPointF GameObject::physicsPosition() const
{
    return m_physicsPosition;
}

void GameObject::setPhysicsPosition(const QPointF &physicsPosition)
{
    if (m_physicsPosition == physicsPosition)
        return;

    qCDebug(dcGameObject()) << this << "Physics position changed" << physicsPosition;
    m_physicsPosition = physicsPosition;
    emit physicsPositionChanged(m_physicsPosition);
}

GameObject::PhysicsFlags GameObject::categoryFlag() const
{
    return m_categoryFlag;
}

void GameObject::setCategoryFlag(GameObject::PhysicsFlags categoryFlag)
{
    if (m_categoryFlag == categoryFlag)
        return;

    qCDebug(dcGameObject()) << this << "Physics flag changed" << categoryFlag;
    m_categoryFlag = categoryFlag;
    emit categoryFlagChanged(m_categoryFlag);
}

GameObject::PhysicsFlags GameObject::collisionFlag() const
{
    return m_collisionFlag;
}

void GameObject::setCollisionFlag(PhysicsFlags collisionFlag)
{
    if (m_collisionFlag == collisionFlag)
        return;

    qCDebug(dcGameObject()) << this << "Collision flag changed" << collisionFlag;
    m_collisionFlag = collisionFlag;
    emit collisionFlagChanged(m_collisionFlag);
}

QQmlListProperty<QPointF> GameObject::vertices() const
{
    return m_vertices;
}

void GameObject::setVertices(const QQmlListProperty<QPointF> &vertices)
{
    if (m_vertices == vertices)
        return;

    m_vertices = vertices;
    emit verticesChanged(m_vertices);
}

QDebug operator<<(QDebug debug, GameObject *gameObject)
{
    debug.nospace() << "GameObject(" << gameObject->name();
    debug.nospace() << ", " << gameObject->position();
    debug.nospace() << ", " << gameObject->size();
    debug.nospace() << ", " << gameObject->shape();
    debug.nospace() << ", " << gameObject->layer();
    debug.nospace() << ") ";
    return debug.space();
}
