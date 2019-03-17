#include "pathcontroller.h"
#include "debugcategories.h"

#include <QtMath>
#include <QDateTime>
#include <QVector2D>

PathController::PathController(QObject *parent) : QObject(parent)
{

}

Path *PathController::path() const
{
    return m_path;
}

void PathController::setPath(Path *path, const QPointF &startPosition)
{
    if (m_path == path)
        return;

    if (m_path) disconnect(m_path, &Path::currentIndexChanged, this, &PathController::onPathCurrentIndexChanged);

    m_path = path;
    emit pathChanged(m_path);

    if (!m_path) return;
    connect(m_path, &Path::currentIndexChanged, this, &PathController::onPathCurrentIndexChanged);

    // Init
    m_startPosition = startPosition;
    m_lastPosition = startPosition;
    m_path->setCurrentIndex(0);
    m_targetPosition = m_startPosition + m_path->currentPathSegment().offset();
}

QPointF PathController::movementVector() const
{
    return m_movementVector;
}

double PathController::angle() const
{
    return m_angle;
}

void PathController::evaluatePosition(const QPointF &position)
{
    if (!m_path) {
        m_movementVector = QPointF(0, 0);
        return;
    }

    // Calculate the distance from the current position to the target position
    //qCDebug(dcPathController()) << "Evaluate path controler for position" << m_path->currentPathSegment();
    switch (m_path->currentPathSegment().type()) {
    case PathSegment::TypeSegment: {
        //qCDebug(dcPathController()) << " --> Target position:" << m_startPosition << position << m_targetPosition;
        QPointF distanceVector = m_targetPosition - position;
        double distanceLength = qSqrt(qPow(distanceVector.x(), 2) + qPow(distanceVector.y(), 2));

        if (distanceLength <= 0.05) {
            qCDebug(dcPathController()) << "Segment finished." << m_path->currentPathSegment() << "Next segment!";
            m_movementVector = QPointF(0, 0);
            m_path->nextSegment();
            break;
        }

        m_angle = qAtan2(distanceVector.y(), distanceVector.x());
        double movementX = m_path->currentPathSegment().speed() * qCos(m_angle);
        double movementY = m_path->currentPathSegment().speed() * qSin(m_angle);
        m_movementVector = QPointF(movementX, movementY);
        //qCDebug(dcPathController()) << "--> Remaining distance vector" << distanceVector << distanceLength << m_angle << m_movementVector;
        break;
    }
    case PathSegment::TypeRotate: {
        m_movementVector = QPointF(0, 0);
        double angle = m_path->currentPathSegment().angle() * M_PI / 180;
        double angleAbsolut = angle;
        if (angleAbsolut > M_PI * 2) {
            angleAbsolut -= M_PI * 2;
        }
        qCDebug(dcPathController()) << "----> Rotate from angle" << m_angle << "by" << angle << angleAbsolut;
        m_angle += angleAbsolut;
        qCDebug(dcPathController()) << "Segment finished." << m_path->currentPathSegment() << "Next segment!";
        m_path->nextSegment();
        break;
    }
    case PathSegment::TypePause:
        m_movementVector = QPointF(0, 0);
        //qCDebug(dcPathController()) << QDateTime::currentMSecsSinceEpoch() - m_pauseStartTimeStamp;
        if (QDateTime::currentMSecsSinceEpoch() - m_pauseStartTimeStamp >= m_path->currentPathSegment().duration()) {
            qCDebug(dcPathController()) << "Segment finished." << m_path->currentPathSegment() << "Next segment!";
            m_path->nextSegment();
        }
        break;
    }
}

void PathController::onPathCurrentIndexChanged(int currentIndex)
{
    qCDebug(dcPathController()) << "Current path segment changed:" << currentIndex << "/" << m_path->pathSegments().count() -1 << "|" << m_path->currentPathSegment();
    switch (m_path->currentPathSegment().type()) {
    case PathSegment::TypeSegment:
        m_lastPosition = m_targetPosition;
        m_targetPosition = m_lastPosition + m_path->currentPathSegment().offset();
        break;
    case PathSegment::TypePause:
        m_pauseStartTimeStamp = QDateTime::currentMSecsSinceEpoch();
        qCDebug(dcPathController()) << "Starting pause segment" << m_pauseStartTimeStamp;
        break;
    case PathSegment::TypeRotate:
        break;
    }

}
