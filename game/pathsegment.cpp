#include "pathsegment.h"

#include <QDebug>

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

int PathSegment::duration() const
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

QDebug operator<<(QDebug debug, const PathSegment &pathSegment)
{
    debug.nospace() << "PathSegment(";
    switch (pathSegment.type()) {
    case PathSegment::TypeSegment:
        debug.nospace() << "Move: Offset: " << pathSegment.offset() << ", speed: " << pathSegment.speed();
        break;
    case PathSegment::TypeRotate:
        debug.nospace() << "Rotat: " << pathSegment.angle();
        break;
    case PathSegment::TypePause:
        debug.nospace() << "Pause: " << pathSegment.duration();
        break;
    }
    debug.nospace() << ")";
    return debug.space();
}
