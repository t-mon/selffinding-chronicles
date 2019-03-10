#include "gameobject.h"

#include <QDebug>

GameObject::GameObject(QObject *parent) : QObject(parent)
{
    qRegisterMetaType<PhysicsFlags>("PhysicsFlags");
}

QString GameObject::name() const
{
    return m_name;
}

void GameObject::setName(const QString &name)
{
    if (m_name == name)
        return;

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

    m_position = position;
    emit positionChanged(m_position);
}

double GameObject::layer() const
{
    return m_layer;
}

void GameObject::setLayer(double layer)
{
    m_layer = layer;
    emit layerChanged(m_layer);
}

QPointF GameObject::centerPosition() const
{
    return m_position + QPointF(m_size.width() / 2.0, m_size.height() / 2.0);
}

QSize GameObject::size() const
{
    return m_size;
}

void GameObject::setSize(const QSize &size)
{
    if (m_size == size)
        return;

    m_size = size;
    emit sizeChanged(m_size);
}

GameObject::Shape GameObject::shape() const
{
    return m_shape;
}

void GameObject::setShape(GameObject::Shape shape)
{
    if (m_shape == shape)
        return;

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

    m_bodyType = bodyType;
    emit bodyTypeChanged(m_bodyType);
}

QSize GameObject::physicsSize() const
{
    return m_physicsSize;
}

void GameObject::setPhysicsSize(const QSize &physicsSize)
{
    if (m_physicsSize == physicsSize)
        return;

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
    debug.nospace() << ", " << gameObject->shape();
    debug.nospace() << ") ";
    return debug;
}
