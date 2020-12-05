#include "chestitem.h"
#include "../debugcategories.h"

ChestItem::ChestItem(QObject *parent) :
    GameItem(parent)
{
    m_items = new GameItems(this);
    setInteraction(InteractionOpen);

    m_lockItem = new LockItem(this);

    connect(m_lockItem, &LockItem::lockedChanged, this, [this](bool locked){
        qCDebug(dcItem()) << this << "is now" << (locked ? "locked" : "unlocked");
        if (locked) {
            setInteraction(InteractionUnlock);
        } else {
            if (m_open) {
                setInteraction(InteractionPlunder);
            } else {
                setInteraction(InteractionOpen);
            }
        }
    });
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

LockItem *ChestItem::lockItem() const
{
    return m_lockItem;
}

bool ChestItem::open() const
{
    return m_open;
}

void ChestItem::setOpen(bool open)
{
    if (m_open == open)
        return;

    m_open = open;
    emit openChanged(m_open);
    if (m_open) {
        setInteraction(InteractionPlunder);
    } else {
        setInteraction(InteractionOpen);
    }
}

QDebug operator<<(QDebug debug, ChestItem *chestItem)
{
    debug.nospace() << "ChestItem(" << chestItem->itemTypeName();
    debug.nospace() << ", " << chestItem->name();
    debug.nospace() << ", " << chestItem->itemId();
    debug.nospace() << ", " << chestItem->position();
    debug.nospace() << ", " << chestItem->size();
    debug.nospace() << ", " << (chestItem->open() ? "open" : "closed");
    debug.nospace() << ", " << (chestItem->lockItem()->locked() ? "locked" : "unlocked");
    debug.nospace() << ", " << chestItem->lockItem()->lockCombination();
    debug.nospace() << ")" << Qt::endl;

    foreach (GameItem *item, chestItem->items()->gameItems()) {
        debug.nospace() << "    --> " << item << Qt::endl;
    }

    return debug.space();
}
