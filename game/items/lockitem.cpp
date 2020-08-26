#include "lockitem.h"
#include "../debugcategories.h"

LockItem::LockItem(QObject *parent) : QObject(parent)
{

}

bool LockItem::locked() const
{
    return m_locked;
}

void LockItem::setLocked(bool locked)
{
    if (m_locked == locked)
        return;

    qCDebug(dcItem()) << "Locked changed to" << locked;
    m_locked = locked;
    emit lockedChanged(m_locked);
}

QStringList LockItem::lockCombination() const
{
    return m_lockCombination;
}

void LockItem::setLockCombination(const QStringList &lockCombination)
{
    if (m_lockCombination == lockCombination)
        return;

    m_lockCombination = lockCombination;
    emit lockCombinationChanged(m_lockCombination);
}

int LockItem::unlockProgress() const
{
    return m_unlockProgress;
}

void LockItem::unlockLeftMovement()
{
    // Note: left = 0; right = 1
    if (!m_locked)
        return;

    if (m_lockCombination.at(m_unlockProgressStep) == "0") {
        qCDebug(dcItem()) << "Unlock left was correct";
        m_unlockProgressStep++;
        setUnlockProgress(static_cast<int>(qRound(100.0 * m_unlockProgressStep / m_lockCombination.count())));
        if (m_unlockProgressStep >= m_lockCombination.count()) {
            setLocked(false);
        }
    } else {
        qCDebug(dcItem()) << "Unlock left was NOT correct";
        resetUnlock();
    }
}

void LockItem::unlockRightMovement()
{
    // Note: left = 0; right = 1
    if (!m_locked)
        return;

    if (m_lockCombination.at(m_unlockProgressStep) == "1") {
        qCDebug(dcItem()) << "Unlock right was correct";
        m_unlockProgressStep++;
        setUnlockProgress(static_cast<int>(qRound(100.0 * m_unlockProgressStep / m_lockCombination.count())));
        if (m_unlockProgressStep >= m_lockCombination.count()) {
            setLocked(false);
        }
    } else {
        qCDebug(dcItem()) << "Unlock right was NOT correct";
        resetUnlock();
    }
}

void LockItem::resetUnlock()
{
    qCDebug(dcItem()) << "Reset unlock";
    m_unlockProgressStep = 0;
    setUnlockProgress(0);
    emit unlockReset();
}

void LockItem::setUnlockProgress(int unlockProgress)
{
    if (m_unlockProgress == unlockProgress)
        return;

    qCDebug(dcItem()) << "Unlock progress changed to" << unlockProgress << "%";
    m_unlockProgress = unlockProgress;
    emit unlockProgressChanged(m_unlockProgress);
}

QDebug operator<<(QDebug debug, LockItem *lockItem)
{
    debug.nospace() << "LockItem(";
    debug.nospace() << ", " << (lockItem->locked() ? "locked" : "unlocked");
    debug.nospace() << ", " << lockItem->lockCombination();
    debug.nospace() << ")";
    return debug.space();
}
