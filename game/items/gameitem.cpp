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
