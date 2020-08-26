#ifndef LOCKITEM_H
#define LOCKITEM_H

#include <QObject>

class LockItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool locked READ locked NOTIFY lockedChanged)
    Q_PROPERTY(QStringList lockCombination READ lockCombination NOTIFY lockCombinationChanged)
    Q_PROPERTY(int unlockProgress READ unlockProgress NOTIFY unlockProgressChanged)

public:
    explicit LockItem(QObject *parent = nullptr);
    ~LockItem() override = default;

    bool locked() const;
    void setLocked(bool locked);

    QStringList lockCombination() const;
    void setLockCombination(const QStringList &lockCombination);

    int unlockProgress() const;

    Q_INVOKABLE void unlockLeftMovement();
    Q_INVOKABLE void unlockRightMovement();

private:
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

QDebug operator<<(QDebug debug, LockItem *lockItem);


#endif // LOCKITEM_H
