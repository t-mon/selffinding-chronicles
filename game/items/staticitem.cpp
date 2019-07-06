#include "staticitem.h"

StaticItem::StaticItem(QObject *parent) : GameItem(parent)
{
    setInteraction(InteractionNone);
    setInventoryInteraction(InteractionNone);
}

QString StaticItem::itemTypeName() const
{
    return "Static";
}

GameItem::Type StaticItem::itemType() const
{
    return GameItem::TypeStatic;
}

void StaticItem::performInteraction()
{

}
