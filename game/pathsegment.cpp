#include "pathsegment.h"

PathSegment::PathSegment()
{

}

QPointF PathSegment::offset() const
{
    return m_offset;
}

void PathSegment::setOffset(const QPointF &offset)
{
    m_offset = offset;
}

double PathSegment::speed() const
{
    return m_speed;
}

void PathSegment::setSpeed(double speed)
{
    m_speed = speed;
}

double PathSegment::angle() const
{
    return m_angle;
}

void PathSegment::setAngle(double angle)
{
    m_angle = angle;
}

int PathSegment::duration()
{
    return m_duration;
}

void PathSegment::setDuration(int duration)
{
    m_duration = duration;
}

PathSegment::Type PathSegment::type() const
{
    return m_type;
}

void PathSegment::setType(PathSegment::Type type)
{
    m_type = type;
}
