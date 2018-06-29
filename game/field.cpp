#include "field.h"

Field::Field(QPoint position, QObject *parent) :
    QObject(parent),
    m_position(position)
{

}

QPoint Field::position() const
{
    return m_position;
}

bool Field::accessible() const
{
    return m_accessible;
}

void Field::setAccessible(bool accessible)
{
    if (m_accessible == accessible)
        return;

    m_accessible = accessible;
    emit accessibleChanged(m_accessible);
}
