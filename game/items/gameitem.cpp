#include "gameitem.h"
#include "../debugcategories.h"


GameItem::GameItem(QObject *parent) :
    GameObject(parent)
{

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
    debug.nospace() << ", " << gameItem->position();
    debug.nospace() << ", " << gameItem->size();
    debug.nospace() << ", price: " << gameItem->price();
    debug.nospace() << ", " << gameItem->interaction();
    debug.nospace() << ") ";
    return debug;
}
