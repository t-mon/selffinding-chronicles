#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QObject>
#include <QPoint>

#include "../gameobject.h"

class GameItem : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(Interaction interaction READ interaction NOTIFY interactionChanged)
    Q_PROPERTY(QString imageName READ imageName NOTIFY imageNameChanged)
    Q_PROPERTY(Type itemType READ itemType CONSTANT)
    Q_PROPERTY(QString itemTypeName READ itemTypeName CONSTANT)
    Q_PROPERTY(int price READ price NOTIFY priceChanged)
    Q_PROPERTY(bool playerFocus READ playerFocus NOTIFY playerFocusChanged)
    Q_PROPERTY(bool playerVisible READ playerVisible NOTIFY playerVisibleChanged)

public:
    enum Type {
        TypeUnknown,
        TypePlant,
        TypeAnimal,
        TypeTree,
        TypeWeapon,
        TypeFirearm,
        TypeTreasureChest,
        TypeDoor,
        TypeTeleportStone,
        TypeBox,
        TypePerson,
        TypeEnemy,
        TypeArmor,
        TypeMagicArtefact,
        TypePotion
    };
    Q_ENUM(Type)

    enum Interaction {
        InteractionNone,
        InteractionPick,
        InteractionOpen,
        InteractionClose,
        InteractionEnter,
        InteractionExit,
        InteractionTalk,
        InteractionAttack,
        InteractionDrop,
        InteractionEat,
        InteractionDrink,
        InteractionUse,
        InteractionRead
    };
    Q_ENUM(Interaction)

    explicit GameItem(QObject *parent = nullptr);
    ~GameItem() override = default;

    virtual Type itemType() const = 0;
    virtual QString itemTypeName() const = 0;

    QString imageName() const;
    void setImageName(const QString &imageName);

    int price() const;
    void setPrice(int price);

    bool playerFocus() const;
    void setPlayerFocus(bool playerFocus);

    bool playerVisible() const;
    void setPlayerVisible(bool playerVisible);

    Interaction interaction() const;

    QList<QPoint> unaccessableMap();
    void setUnaccessableMap(const QList<QPoint> unaccessableMap);

    QList<QPoint> visibilityMap();
    void setVisiblilityMap(const QList<QPoint> visiblilityMap);

    virtual void performInteraction() = 0;

    static QString interactionToString(const Interaction &interaction);

private:
    QString m_imageName;
    int m_price = 0;
    bool m_playerFocus = false;
    bool m_playerVisible = false;

    QList<QPoint> m_unaccessableMap;
    QList<QPoint> m_visibilityMap;

protected:
    Interaction m_interaction = InteractionNone;
    void setInteraction(Interaction interaction);

signals:
    void imageNameChanged(const QString &imageName);
    void interactionChanged(Interaction interaction);
    void priceChanged(int price);
    void playerFocusChanged(bool playerFocus);
    void playerVisibleChanged(bool playerVisible);

};

QDebug operator<<(QDebug debug, GameItem *gameItem);

#endif // GAMEITEM_H
