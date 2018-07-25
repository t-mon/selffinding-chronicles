#include "treeitem.h"

TreeItem::TreeItem(QObject *parent) : GameItem(parent)
{

}

QString TreeItem::itemTypeName() const
{
    return tr("Tree");
}

GameItem::Type TreeItem::itemType() const
{
    return Type::TypeTree;
}

void TreeItem::performInteraction()
{

}
