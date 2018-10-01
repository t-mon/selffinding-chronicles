#ifndef PLANTITEM_H
#define PLANTITEM_H

#include <QObject>

#include "gameitem.h"

class PlantItem : public GameItem
{
    Q_OBJECT

public:
    explicit PlantItem(QObject *parent = nullptr);
    ~PlantItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    int healing() const;
    void setHealing(int healing);

    int mana() const;
    void setMana(int mana);

    int speed() const;
    void setSpeed(int speed);

private:
    int m_healing = 0;
    int m_mana = 0;
    int m_speed = 0;

signals:
    void healingChanged(int healing);
    void manaChanged(int mana);
    void speedChanged(int speed);

public slots:
};

#endif // PLANTITEM_H