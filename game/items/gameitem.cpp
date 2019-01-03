#include "gameitem.h"
#include "../debugcategories.h"


GameItem::GameItem(QObject *parent) :
    GameObject(parent)
{

}

QString GameItem::itemId() const
{
    return m_itemId;
}

void GameItem::setItemId(const QString &itemId)
{
    m_itemId = itemId;
}

QString GameItem::imageName() const
{
    return m_imageName;
}

void GameItem::setImageName(const QString &imageName)
{
    if (m_imageName == imageName)
        return;

    qCDebug(dcItem()) << itemTypeName() << "image name changed" << imageName;
    m_imageName = imageName;
    emit imageNameChanged(m_imageName);
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

QList<QPoint> GameItem::unaccessableMap()
{
    return m_unaccessableMap;
}

void GameItem::setUnaccessableMap(const QList<QPoint> unaccessableMap)
{
    m_unaccessableMap = unaccessableMap;
}

QList<QPoint> GameItem::visibilityMap()
{
    return m_visibilityMap;
}

void GameItem::setVisiblilityMap(const QList<QPoint> visiblilityMap)
{
    m_visibilityMap = visiblilityMap;
}

QString GameItem::interactionToString(const GameItem::Interaction &interaction)
{
    QString interactionString;
    switch (interaction) {
    case InteractionNone:
        //: This string describes an interaction for an item. In this case, no interaction
        interactionString = tr("-");
        break;
    case InteractionPick:
        //: This string describes an interaction for an item.
        interactionString = tr("Pick up");
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
    }

    return interactionString;
}

void GameItem::setInteraction(GameItem::Interaction interaction)
{
    if (m_interaction == interaction)
        return;

    qCDebug(dcItem()) << itemTypeName() << "interaction changed" << interaction;
    m_interaction = interaction;
    emit interactionChanged(m_interaction);
}

QDebug operator<<(QDebug debug, GameItem *gameItem)
{
    debug.nospace() << "GameItem(" << gameItem->itemTypeName();
    debug.nospace() << ", " << gameItem->name();
    debug.nospace() << ", " << gameItem->itemId();
    debug.nospace() << ", " << gameItem->position();
    debug.nospace() << ", " << gameItem->size();
    debug.nospace() << ", price: " << gameItem->price();
    debug.nospace() << ", " << GameItem::interactionToString(gameItem->interaction());
    debug.nospace() << ")";
    return debug.space();
}
