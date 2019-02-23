#ifndef PATHSEGMENT_H
#define PATHSEGMENT_H

#include <QPointF>

class PathSegment
{

public:
    enum Type {
        TypeSegment,
        TypePause,
        TypeRotate
    };

    explicit PathSegment();

    QPointF offset() const;
    void setOffset(const QPointF &offset);

    double speed() const;
    void setSpeed(double speed);

    double angle() const;
    void setAngle(double angle);

    int duration();
    void setDuration(int duration);

    Type type() const;
    void setType(Type type);

private:
    QPointF m_offset;
    double m_speed = 1.0;
    double m_angle = 0;
    int m_duration = 1000;
    Type m_type = TypeSegment;

};

#endif // PATHSEGMENT_H
