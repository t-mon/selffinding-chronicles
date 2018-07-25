#ifndef TREEITEM_H
#define TREEITEM_H

#include <QObject>

#include "gameitem.h"

class TreeItem : public GameItem
{
    Q_OBJECT
public:
    explicit TreeItem(QObject *parent = nullptr);

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

};

#endif // TREEITEM_H
