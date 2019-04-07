#include "staticitem.h"

StaticItem::StaticItem(QObject *parent) : GameItem(parent)
{

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
