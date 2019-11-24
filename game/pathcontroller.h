#ifndef PATHFOLLOWINGCONTROLLER_H
#define PATHFOLLOWINGCONTROLLER_H

#include <QObject>

#include "path.h"

class PathController : public QObject
{
    Q_OBJECT
public:
    explicit PathController(QObject *parent = nullptr);

    Path *path() const;
    void setPath(Path *path, const QPointF &startPosition);

    QPointF movementVector() const;
    double angle() const;

    void evaluatePosition(const QPointF &position);

private:
    Path *m_path = nullptr;

    QPointF m_startPosition = QPointF(0, 0);
    QPointF m_lastPosition = QPointF(0, 0);
    QPointF m_targetPosition = QPointF(0, 0);

    // Current movement
    QPointF m_movementVector = QPointF(0, 0);
    double m_angle = 0;

    qint64 m_pauseStartTimeStamp = 0;
    qint64 m_pauseLastTimeStamp = 0;

signals:
    void pathChanged(Path *path);

private slots:
    void onPathCurrentIndexChanged(int currentIndex);

};

#endif // PATHFOLLOWINGCONTROLLER_H
