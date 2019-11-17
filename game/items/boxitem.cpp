#include "boxitem.h"

BoxItem::BoxItem(QObject *parent) :
    GameItem(parent)
{

}

QString BoxItem::itemTypeName() const
{
    return "Box";
}

GameItem::Type BoxItem::itemType() const
{
    return GameItem::TypeBox;
}

void BoxItem::performInteraction()
{

}

QDebug operator<<(QDebug debug, BoxItem *box)
{
    debug.nospace() << "Box(" << box->itemTypeName();
    debug.nospace() << ", " << box->name();
    debug.nospace() << ", " << box->itemId();
    debug.nospace() << ", " << box->position();
    debug.nospace() << ", " << box->size();
    debug.nospace() << ")";
    return debug.space();
}
