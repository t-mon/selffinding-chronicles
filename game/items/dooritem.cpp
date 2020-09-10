#include "dooritem.h"
#include "../debugcategories.h"

DoorItem::DoorItem(QObject *parent) :
    PassageItem(parent)
{
    setInteraction(InteractionOpen);
}

QString DoorItem::itemTypeName() const
{
    return tr("Door");
}

GameItem::Type DoorItem::itemType() const
{
    return GameItem::TypeDoor;
}

void DoorItem::performInteraction()
{

}

bool DoorItem::open() const
{
    return m_open;
}

void DoorItem::setOpen(bool open)
{
    if (m_open == open)
        return;

    m_open = open;
    emit openChanged(m_open);

    if (m_open) {
        setInteraction(InteractionClose);
    } else {
        if (m_locked) {
            setInteraction(InteractionUnlock);
        } else {
            setInteraction(InteractionOpen);
        }
    }
}

bool DoorItem::locked() const
{
    return m_locked;
}

void DoorItem::setLocked(bool locked)
{
    if (m_locked == locked)
        return;

    qCDebug(dcItem()) << itemTypeName() << name() << "locked changed to" << locked;
    m_locked = locked;
    emit lockedChanged(m_locked);

    if (m_locked) {
        setInteraction(InteractionUnlock);
    } else {
        if (m_open) {
            setInteraction(InteractionClose);
        } else {
            setInventoryInteraction(InteractionClose);
        }
    }
}

QStringList DoorItem::lockCombination() const
{
    return m_lockCombination;
}

void DoorItem::setLockCombination(const QStringList &lockCombination)
{
    if (m_lockCombination == lockCombination)
        return;

    m_lockCombination = lockCombination;
    emit lockCombinationChanged(m_lockCombination);
}

int DoorItem::unlockProgress() const
{
    return m_unlockProgress;
}

void DoorItem::unlockLeftMovement()
{
    // Note: left = 0; right = 1
    if (!m_locked)
        return;

    if (m_lockCombination.at(m_unlockProgressStep) == "0") {
        qCDebug(dcItem()) << itemTypeName() << name() << "unlock left was correct";
        m_unlockProgressStep++;
        setUnlockProgress(static_cast<int>(qRound(100.0 * m_unlockProgressStep / m_lockCombination.count())));
        if (m_unlockProgressStep >= m_lockCombination.count()) {
            setLocked(false);
        }
    } else {
        qCDebug(dcItem()) << itemTypeName() << name() << "unlock left was NOT correct";
        resetUnlock();
    }
}

void DoorItem::unlockRightMovement()
{
    // Note: left = 0; right = 1
    if (!m_locked)
        return;

    if (m_lockCombination.at(m_unlockProgressStep) == "1") {
        qCDebug(dcItem()) << itemTypeName() << name() << "unlock right was correct";
        m_unlockProgressStep++;
        setUnlockProgress(static_cast<int>(qRound(100.0 * m_unlockProgressStep / m_lockCombination.count())));
        if (m_unlockProgressStep >= m_lockCombination.count()) {
            setLocked(false);
        }
    } else {
        qCDebug(dcItem()) << itemTypeName() << name() << "unlock right was NOT correct";
        resetUnlock();
    }
}

void DoorItem::resetUnlock()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "reset unlock";
    m_unlockProgressStep = 0;
    setUnlockProgress(0);
    emit unlockReset();
}

void DoorItem::setUnlockProgress(int unlockProgress)
{
    if (m_unlockProgress == unlockProgress)
        return;

    qCDebug(dcItem()) << itemTypeName() << name() << "unlock progress changed to" << unlockProgress << "%";
    m_unlockProgress = unlockProgress;
    emit unlockProgressChanged(m_unlockProgress);
}

QDebug operator<<(QDebug debug, DoorItem *doorItem)
{
    debug.nospace() << "DoorItem(" << doorItem->itemTypeName();
    debug.nospace() << ", " << doorItem->targetMap();
    debug.nospace() << ", " << doorItem->targetPosition();
    debug.nospace() << ", " << (doorItem->open() ? "open" : "closed");
    debug.nospace() << ", " << (doorItem->locked() ? "locked" : "unlocked");
    debug.nospace() << ", " << doorItem->lockCombination();
    debug.nospace() << ")" << Qt::endl;
    return debug.space();
}
