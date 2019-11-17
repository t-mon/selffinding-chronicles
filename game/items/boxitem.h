#ifndef BOXITEM_H
#define BOXITEM_H

#include <QObject>

#include "gameitem.h"
#include "../debugcategories.h"

class BoxItem : public GameItem
{
    Q_OBJECT

public:
    explicit BoxItem(QObject *parent = nullptr);
    ~BoxItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    void performInteraction() override;

};

QDebug operator<<(QDebug debug, BoxItem *box);

#endif // BOXITEM_H
