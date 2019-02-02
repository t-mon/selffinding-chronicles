#ifndef CHESTITEM_H
#define CHESTITEM_H

#include <QObject>

#include "gameitemsproxy.h"

class ChestItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(GameItems *items READ items CONSTANT)
    Q_PROPERTY(bool locked READ locked NOTIFY lockedChanged)
    Q_PROPERTY(QStringList lockCombination READ lockCombination NOTIFY lockCombinationChanged)
    Q_PROPERTY(int unlockProgress READ unlockProgress NOTIFY unlockProgressChanged)

public:
    explicit ChestItem(QObject *parent = nullptr);
    ~ChestItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    GameItems *items() const;

    bool locked() const;
    void setLocked(bool locked);

    QStringList lockCombination() const;
    void setLockCombination(const QStringList &lockCombination);

    int unlockProgress() const;

    Q_INVOKABLE void unlockLeftMovement();
    Q_INVOKABLE void unlockRightMovement();

private:
    GameItems *m_items = nullptr;
    bool m_locked = false;
    QStringList m_lockCombination;

    int m_unlockProgress = 0;
    int m_unlockProgressStep = 0;

    void resetUnlock();
    void setUnlockProgress(int unlockProgress);

signals:
    void lockedChanged(bool locked);
    void lockCombinationChanged(const QStringList &lockCombination);

    void unlockProgressChanged(int unlockProgress);
    void successfullUnlockMove();
    void unlockReset();
};

QDebug operator<<(QDebug debug, ChestItem *chestItem);

#endif // CHESTITEM_H
