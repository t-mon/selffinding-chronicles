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
    Q_PROPERTY(bool playerVisible READ playerVisible WRITE setPlayerVisible NOTIFY playerVisibleChanged)
    Q_PROPERTY(bool hidingPlayer READ hidingPlayer NOTIFY hidingPlayerChanged)
    Q_PROPERTY(bool playerOnItem READ playerOnItem WRITE setPlayerOnItem NOTIFY playerOnItemChanged)

public:
    enum Type {
        TypeNone,
        TypePlant,
        TypeAnimal,
        TypeTree,
        TypeWeapon,
        TypeFirearm,
        TypeChest,
        TypeDoor,
        TypeTeleportStone,
        TypeBox,
        TypeCharacter,
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

    enum PhysicsFlag {
        PhysicsCharacter = 0x0001,
        PhysicsEnemy = 0x0002,
        PhysicsPickableItem = 0x0004,
        PhysicsStaticItem = 0x0008,
        PhysicsWalkableItem = 0x0010,
        PhysicsSensor = 0x0020,
        PhysicsCategory7 = 0x0040,
        PhysicsCategory8 = 0x0080,
        PhysicsCategory9 = 0x0100,
        PhysicsCategory10 = 0x0200,
        PhysicsCategory11 = 0x0400,
        PhysicsCategory12 = 0x0800,
        PhysicsCategory13 = 0x1000,
        PhysicsCategory14 = 0x2000,
        PhysicsCategory15 = 0x4000,
        PhysicsCategory16 = 0x8000,
        PhysicsAll = 0xFFFF,
        PhysicsNone =0x0000
    };
    Q_ENUM(PhysicsFlag)
    Q_DECLARE_FLAGS(PhysicsFlags, PhysicsFlag)

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

    bool hidingPlayer() const;
    void setHidingPlayer(bool hidingPlayer);

    bool playerOnItem() const;
    void setPlayerOnItem(bool playerOnItem);

    Interaction interaction() const;

    QList<QPoint> unaccessableMap();
    void setUnaccessableMap(const QList<QPoint> unaccessableMap);

    QList<QPoint> visibilityMap();
    void setVisiblilityMap(const QList<QPoint> visiblilityMap);

    virtual void performInteraction() = 0;

    static QString interactionToString(const Interaction &interaction);

private:
    QString m_imageName = "/images/world/base/placeholder.png";
    int m_price = 0;
    bool m_playerFocus = false;
    bool m_playerVisible = false;
    bool m_hidingPlayer = false;
    bool m_playerOnItem = false;

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
    void hidingPlayerChanged(bool hidingPlayer);
    void playerOnItemChanged(bool playerOnItem);

    // Interaction signals
    void picked();
    void opened();
    void closed();

};

QDebug operator<<(QDebug debug, GameItem *gameItem);

#endif // GAMEITEM_H
