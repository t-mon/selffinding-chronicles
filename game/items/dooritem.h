#ifndef DOORITEM_H
#define DOORITEM_H

#include <QObject>

#include "passageitem.h"

class DoorItem : public PassageItem
{
    Q_OBJECT
    Q_PROPERTY(bool open READ open WRITE setOpen NOTIFY openChanged)
    Q_PROPERTY(bool locked READ locked NOTIFY lockedChanged)
    Q_PROPERTY(QStringList lockCombination READ lockCombination NOTIFY lockCombinationChanged)
    Q_PROPERTY(int unlockProgress READ unlockProgress NOTIFY unlockProgressChanged)

public:
    explicit DoorItem(QObject *parent = nullptr);
    ~DoorItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    bool open() const;
    void setOpen(bool open);

    bool locked() const;
    void setLocked(bool locked);

    QStringList lockCombination() const;
    void setLockCombination(const QStringList &lockCombination);

    int unlockProgress() const;

    Q_INVOKABLE void unlockLeftMovement();
    Q_INVOKABLE void unlockRightMovement();

private:
    bool m_open = false;
    bool m_locked = false;
    QStringList m_lockCombination;

    int m_unlockProgress = 0;
    int m_unlockProgressStep = 0;

    void resetUnlock();
    void setUnlockProgress(int unlockProgress);

signals:
    void openChanged(bool open);
    void lockedChanged(bool locked);
    void lockCombinationChanged(const QStringList &lockCombination);

    void unlockProgressChanged(int unlockProgress);
    void successfullUnlockMove();
    void unlockReset();
};

QDebug operator<<(QDebug debug, DoorItem *doorItem);

#endif // DOORITEM_H
