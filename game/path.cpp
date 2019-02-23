#include "path.h"




Path::Path(QObject *parent) : QObject(parent)
{

}

QString Path::id() const
{
    return m_id;
}

void Path::setId(const QString &id)
{
    m_id = id;
}

QList<PathSegment> Path::pathSegments() const
{
    return m_pathSegments;
}

void Path::setPathSegments(const QList<PathSegment> &pathSegments)
{
    m_pathSegments = pathSegments;
}

int Path::currentIndex() const
{
    return m_currentIndex;
}

void Path::setCurrentIndex(int currentIndex)
{
    if (m_currentIndex == currentIndex)
        return;

    m_currentIndex = currentIndex;
    emit currentIndexChanged(m_currentIndex);
}

QDebug operator<<(QDebug debug, Path *path)
{
    debug.nospace() << "Path(" << path->id() << ")" << endl;
    for (int i = 0; i < path->pathSegments().count(); i++) {
        debug.nospace() << "    [" << i << "] ";
        switch (path->pathSegments()[i].type()) {
        case PathSegment::TypeSegment:
            debug.nospace() << "segment: offset " << path->pathSegments()[i].offset() << ", speed:" << path->pathSegments()[i].speed()  << endl;
            break;
        case PathSegment::TypePause:
            debug.nospace() << "pause: duration" << path->pathSegments()[i].duration() << endl;
            break;
        case PathSegment::TypeRotate:
            debug.nospace() << "rotate: angle" << path->pathSegments()[i].angle() << endl;
            break;
        }
    }
    return debug.space();
}
