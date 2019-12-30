#ifndef SAVEGAMEPROXY_H
#define SAVEGAMEPROXY_H

#include <QObject>

class SaveGameProxy : public QObject
{
    Q_OBJECT
public:
    explicit SaveGameProxy(QObject *parent = nullptr);

signals:

};

#endif // SAVEGAMEPROXY_H
