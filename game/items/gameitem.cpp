#include "gameitem.h"
#include "../debugcategories.h"


GameItem::GameItem(QObject *parent) :
    GameObject(parent)
{

}

int GameItem::price() const
{
    return m_price;
}

void GameItem::setPrice(int price)
{
    if (m_price == price)
        return;

    m_price = price;
    emit priceChanged(m_price);
}

bool GameItem::playerFocus() const
{
    return m_playerFocus;
}

void GameItem::setPlayerFocus(bool playerFocus)
{
    if (m_playerFocus == playerFocus)
        return;

    qCDebug(dcItem()) << "Focus changed" << playerFocus << this;
    m_playerFocus = playerFocus;
    emit playerFocusChanged(m_playerFocus);
}

bool GameItem::playerVisible() const
{
    return m_playerVisible;
}

void GameItem::setPlayerVisible(bool playerVisible)
{
    if (m_playerVisible == playerVisible)
        return;

    qCDebug(dcItem()) << "Player visible changed" << playerVisible << this;
    m_playerVisible = playerVisible;
    emit playerVisibleChanged(m_playerVisible);
}

bool GameItem::hidingPlayer() const
{
    return m_hidingPlayer;
}

void GameItem::setHidingPlayer(bool hidingPlayer)
{
    if (m_hidingPlayer == hidingPlayer)
        return;

    m_hidingPlayer = hidingPlayer;
    emit hidingPlayerChanged(m_hidingPlayer);
}

bool GameItem::playerOnItem() const
{
    return m_playerOnItem;
}

void GameItem::setPlayerOnItem(bool playerOnItem)
{
    if (m_playerOnItem == playerOnItem)
        return;

    qCDebug(dcItem()) << "Player on item changed" << playerOnItem << this;
    m_playerOnItem = playerOnItem;
    emit playerOnItemChanged(m_playerOnItem);
}

GameItem::Interaction GameItem::interaction() const
{
    return m_interaction;
}

void GameItem::setInteraction(GameItem::Interaction interaction)
{
    if (m_interaction == interaction)
        return;

    qCDebug(dcItem()) << itemTypeName() << "interaction changed" << interaction;
    m_interaction = interaction;
    emit interactionChanged(m_interaction);
}

GameItem::Interaction GameItem::inventoryInteraction() const
{
    return m_inventoryInteraction;
}

void GameItem::setInventoryInteraction(GameItem::Interaction interaction)
{
    if (m_inventoryInteraction == interaction)
        return;

    m_inventoryInteraction = interaction;
    emit inventoryInteractionChanged(m_inventoryInteraction);
}


QString GameItem::interactionString() const
{
    return interactionToString(m_interaction);
}

QString GameItem::interactionToString(Interaction interaction)
{
    QString interactionString;
    switch (interaction) {
    case InteractionNone:
        //: This string describes an interaction for an item. In this case, no interaction
        interactionString = tr("No interaction");
        break;
    case InteractionPick:
        //: This string describes an interaction for an item.
        interactionString = tr("Pick up");
        break;
    case InteractionArm:
        //: This string describes an interaction for an item.
        interactionString = tr("Arm");
        break;
    case InteractionOpen:
        //: This string describes an interaction for an item.
        interactionString = tr("Open");
        break;
    case InteractionClose:
        //: This string describes an interaction for an item.
        interactionString = tr("Close");
        break;
    case InteractionEnter:
        //: This string describes an interaction for an item.
        interactionString = tr("Enter");
        break;
    case InteractionExit:
        //: This string describes an interaction for an item.
        interactionString = tr("Exit");
        break;
    case InteractionTalk:
        //: This string describes an interaction for an item.
        interactionString = tr("Talk");
        break;
    case InteractionAttack:
        //: This string describes an interaction for an item.
        interactionString = tr("Attack");
        break;
    case InteractionDrop:
        //: This string describes an interaction for an item.
        interactionString = tr("Drop");
        break;
    case InteractionEat:
        //: This string describes an interaction for an item.
        interactionString = tr("Eat");
        break;
    case InteractionDrink:
        //: This string describes an interaction for an item.
        interactionString = tr("Drink");
        break;
    case InteractionUse:
        //: This string describes an interaction for an item.
        interactionString = tr("Use");
        break;
    case InteractionRead:
        //: This string describes an interaction for an item.
        interactionString = tr("Read");
        break;
    case InteractionPlunder:
        //: This string describes an interaction for an item.
        interactionString = tr("Plunder");
        break;
    case InteractionUnlock:
        //: This string describes an interaction for an item.
        interactionString = tr("Unlock");
        break;
    }

    return interactionString;
}

QDebug operator<<(QDebug debug, GameItem *gameItem)
{
    debug.nospace() << "GameItem(" << gameItem->itemTypeName();
    debug.nospace() << ", " << gameItem->name();
    debug.nospace() << ", " << gameItem->resourcePath();
    debug.nospace() << ", " << gameItem->position();
    debug.nospace() << ", " << gameItem->size();
    debug.nospace() << ", price: " << gameItem->price();
    debug.nospace() << ", " << GameItem::interactionToString(gameItem->interaction());
    debug.nospace() << ")";
    return debug.space();
}
