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

    int duration() const;
    void setDuration(int duration);

    Type type() const;
    void setType(Type type);

private:
    QPointF m_offset = QPointF(0, 0);
    double m_speed = 0;
    double m_angle = 0;
    int m_duration = 1000;
    Type m_type = TypeSegment;

};

QDebug operator<<(QDebug debug, const PathSegment &pathSegment);

#endif // PATHSEGMENT_H
