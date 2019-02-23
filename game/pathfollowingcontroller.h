#ifndef PATHFOLLOWINGCONTROLLER_H
#define PATHFOLLOWINGCONTROLLER_H

#include <QObject>
#include "path.h"

class PathFollowingController : public QObject
{
    Q_OBJECT
public:
    explicit PathFollowingController(QObject *parent = nullptr);

    Path *path() const;
    void setPath(Path *path);

private:
    Path *m_path = nullptr;

signals:
    void pathChanged(Path *path);

public slots:

};

#endif // PATHFOLLOWINGCONTROLLER_H
