#include "pathfollowingcontroller.h"

PathFollowingController::PathFollowingController(QObject *parent) : QObject(parent)
{

}

Path *PathFollowingController::path() const
{
    return m_path;
}

void PathFollowingController::setPath(Path *path)
{
    if (m_path == path)
        return;

    m_path = path;
    emit pathChanged(m_path);
}
