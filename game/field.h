#ifndef FIELD_H
#define FIELD_H

#include <QPoint>
#include <QObject>

class Field : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QPoint position READ position NOTIFY positionChanged)
    Q_PROPERTY(bool accessible READ accessible WRITE setAccessible NOTIFY accessibleChanged)

public:
    explicit Field(QPoint position, QObject *parent = nullptr);

    QPoint position() const;

    bool accessible() const;
    void setAccessible(bool accessible);

private:
    QPoint m_position;
    bool m_accessible = true;

signals:
    void positionChanged(const QPoint &position);
    void accessibleChanged(bool accessible);

};

#endif // FIELD_H
