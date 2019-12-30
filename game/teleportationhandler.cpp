#include "teleportationhandler.h"
#include "debugcategories.h"
#include "game.h"

TeleportationHandler::TeleportationHandler(QObject *parent) : QObject(parent)
{

}

TeleportationHandler::TeleportState TeleportationHandler::teleportState() const
{
    return m_teleportState;
}

void TeleportationHandler::teleportAppearAnimationFinished()
{

}

void TeleportationHandler::teleportDisappearAnimationFinished()
{

}

void TeleportationHandler::reset()
{
    qCDebug(dcTeleportation()) << "Reset handler";
    m_currentTeleportItem = nullptr;
    setTeleportState(TeleportStateNone);
}

void TeleportationHandler::setTeleportState(TeleportationHandler::TeleportState teleportState)
{
    if (m_teleportState == teleportState)
        return;

    m_teleportState = teleportState;
    emit teleportStateChanged(m_teleportState);

    qCDebug(dcTeleportation()) << "State changed" << m_teleportState;

    switch (m_teleportState) {
    case TeleportStateNone:
        break;
    case TeleportStateDisppear:
        break;
    case TeleportStateWorldDestruct: {
        if (m_currentTeleportItem->targetMap() == Game::instance()->engine()->dataManager()->map()->resourceFileName()) {
            qCDebug(dcTeleportation()) << "Teleport within the current map";
            setTeleportState(TeleportStateLoading);
        } else {
            qCDebug(dcTeleportation()) << "Teleport to other map. Loading required...";
            // TODO: save current map before loading the new one
        }
        break;
    }
    case TeleportStateLoading:
        if (m_currentTeleportItem->targetMap() == Game::instance()->engine()->dataManager()->map()->resourceFileName()) {
            // Just update the player position, evaluate stuff and construct the world again
            Game::instance()->engine()->player()->setPosition(m_currentTeleportItem->targetPosition());
            setTeleportState(TeleportStateWorldConstruct);
        } else {

        }

        break;
    case TeleportStateWorldConstruct:
        setTeleportState(TeleportStateAppear);
        break;
    case TeleportStateAppear:
        break;
    }
}

void TeleportationHandler::startTeleportation(TeleporterItem *teleportItem)
{
    m_currentTeleportItem = teleportItem;
    setTeleportState(TeleportStateDisppear);
}
