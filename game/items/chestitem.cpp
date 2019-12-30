#include "chestitem.h"
#include "../debugcategories.h"

ChestItem::ChestItem(QObject *parent) :
    GameItem(parent)
{
    m_items = new GameItems(this);
    setInteraction(InteractionOpen);
}

QString ChestItem::itemTypeName() const
{
    return tr("Chest");
}

GameItem::Type ChestItem::itemType() const
{
    return GameItem::TypeChest;
}

void ChestItem::performInteraction()
{

}

GameItems *ChestItem::items() const
{
    return m_items;
}

bool ChestItem::locked() const
{
    return m_locked;
}

void ChestItem::setLocked(bool locked)
{
    if (m_locked == locked)
        return;

    qCDebug(dcItem()) << itemTypeName() << name() << "locked changed to" << locked;
    m_locked = locked;
    emit lockedChanged(m_locked);
}

QStringList ChestItem::lockCombination() const
{
    return m_lockCombination;
}

void ChestItem::setLockCombination(const QStringList &lockCombination)
{
    if (m_lockCombination == lockCombination)
        return;

    m_lockCombination = lockCombination;
    emit lockCombinationChanged(m_lockCombination);
}

int ChestItem::unlockProgress() const
{
    return m_unlockProgress;
}

void ChestItem::unlockLeftMovement()
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

void ChestItem::unlockRightMovement()
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

void ChestItem::resetUnlock()
{
    qCDebug(dcItem()) << itemTypeName() << name() << "reset unlock";
    m_unlockProgressStep = 0;
    setUnlockProgress(0);
    emit unlockReset();
}

void ChestItem::setUnlockProgress(int unlockProgress)
{
    if (m_unlockProgress == unlockProgress)
        return;

    qCDebug(dcItem()) << itemTypeName() << name() << "unlock progress changed to" << unlockProgress << "%";
    m_unlockProgress = unlockProgress;
    emit unlockProgressChanged(m_unlockProgress);
}

QDebug operator<<(QDebug debug, ChestItem *chestItem)
{
    debug.nospace() << "ChestItem(" << chestItem->itemTypeName();
    debug.nospace() << ", " << chestItem->name();
    debug.nospace() << ", " << chestItem->itemId();
    debug.nospace() << ", " << chestItem->position();
    debug.nospace() << ", " << chestItem->size();
    debug.nospace() << ", " << (chestItem->locked() ? "locked" : "unlocked");
    debug.nospace() << ", " << chestItem->lockCombination();
    debug.nospace() << ")" << endl;

    foreach (GameItem *item, chestItem->items()->gameItems()) {
        debug.nospace() << "    --> " << item << endl;
    }

    return debug.space();
}
