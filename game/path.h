#ifndef PATH_H
#define PATH_H

#include <QDebug>
#include <QObject>

#include "pathsegment.h"

class Path : public QObject
{
    Q_OBJECT
public:
    explicit Path(QObject *parent = nullptr);

    QString id() const;
    void setId(const QString &id);

    QList<PathSegment> pathSegments() const;
    void setPathSegments(const QList<PathSegment> &pathSegments);

    PathSegment currentPathSegment() const;

    int currentIndex() const;
    void setCurrentIndex(int currentIndex);

private:
    QString m_id;
    QList<PathSegment> m_pathSegments;
    int m_currentIndex = 0;

signals:
    void currentIndexChanged(int currentIndex);

public slots:
    void nextSegment();
    void previousSegment();


};

QDebug operator<<(QDebug debug, Path *path);

#endif // PATH_H
