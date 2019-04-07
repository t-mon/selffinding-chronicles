#ifndef STATICITEM_H
#define STATICITEM_H

#include <QObject>

#include "gameitem.h"

class StaticItem : public GameItem
{
    Q_OBJECT
public:
    explicit StaticItem(QObject *parent = nullptr);
    ~StaticItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

};

#endif // STATICITEM_H
