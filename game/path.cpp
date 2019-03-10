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

PathSegment Path::currentPathSegment() const
{
    return m_pathSegments[m_currentIndex];
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

void Path::nextSegment()
{
    int nextIndex = m_currentIndex + 1;
    if (nextIndex >= m_pathSegments.count()) {
        setCurrentIndex(0);
    } else {
        setCurrentIndex(nextIndex);
    }
}

void Path::previousSegment()
{
    int previousIndex = m_currentIndex--;
    if (previousIndex <= 0) {
        setCurrentIndex(m_pathSegments.count() - 1);
    } else {
        setCurrentIndex(previousIndex);
    }
}

QDebug operator<<(QDebug debug, Path *path)
{
    debug.nospace() << "Path(" << path->id() << ")" << endl;
    for (int i = 0; i < path->pathSegments().count(); i++) {
        debug.nospace() << "    [" << i << "] " << path->pathSegments()[i];
        if (i < path->pathSegments().count() -1) debug.nospace() << endl;
    }
    return debug.space();
}
