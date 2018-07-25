#include "gameobject.h"

#include <QDebug>

GameObject::GameObject(QObject *parent) : QObject(parent)
{

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

QDebug operator<<(QDebug debug, GameObject *gameObject)
{
    debug.nospace() << "GameObject(" << gameObject->name();
    debug.nospace() << ", " << gameObject->position();
    debug.nospace() << ") ";
    return debug;
}
