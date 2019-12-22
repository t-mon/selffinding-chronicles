#ifndef TELEPORTATIONHANDLER_H
#define TELEPORTATIONHANDLER_H

#include <QObject>

#include "items/teleporteritem.h"

class TeleportationHandler : public QObject
{
    Q_OBJECT
    Q_PROPERTY(TeleportState teleportState READ teleportState NOTIFY teleportStateChanged)

public:
    enum TeleportState {
        TeleportStateNone,
        TeleportStateDisppear,
        TeleportStateWorldDestruct,
        TeleportStateLoading,
        TeleportStateWorldConstruct,
        TeleportStateAppear
    };
    Q_ENUM(TeleportState)

    explicit TeleportationHandler(QObject *parent = nullptr);

    TeleportState teleportState() const;

    // Teleport
    Q_INVOKABLE void teleportAppearAnimationFinished();
    Q_INVOKABLE void teleportDisappearAnimationFinished();

    void reset();

private:
    TeleportState m_teleportState = TeleportStateNone;
    TeleporterItem *m_currentTeleportItem = nullptr;

    void setTeleportState(TeleportState teleportState);

signals:
    void teleportStateChanged(TeleportState teleportState);

public slots:
    void startTeleportation(TeleporterItem *teleportItem);


};

#endif // TELEPORTATIONHANDLER_H
