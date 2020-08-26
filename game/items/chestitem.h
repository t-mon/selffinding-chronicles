#ifndef CHESTITEM_H
#define CHESTITEM_H

#include <QObject>

#include "lockitem.h"
#include "gameitemsproxy.h"

class ChestItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(GameItems *items READ items CONSTANT)
    Q_PROPERTY(LockItem *lockItem READ lockItem CONSTANT)

public:
    explicit ChestItem(QObject *parent = nullptr);
    ~ChestItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    GameItems *items() const;
    LockItem *lockItem() const;

    bool open() const;
    void setOpen(bool open);

private:
    LockItem *m_lockItem = nullptr;
    GameItems *m_items = nullptr;
    bool m_open = false;

signals:
    void openChanged(bool open);
};

QDebug operator<<(QDebug debug, ChestItem *chestItem);

#endif // CHESTITEM_H
