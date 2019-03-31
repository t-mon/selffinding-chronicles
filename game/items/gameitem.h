#ifndef GAMEITEM_H
#define GAMEITEM_H

#include <QObject>
#include <QPoint>

#include "../gameobject.h"

class GameItem : public GameObject
{
    Q_OBJECT
    Q_PROPERTY(Interaction interaction READ interaction NOTIFY interactionChanged)
    Q_PROPERTY(QString interactionString READ interactionString NOTIFY interactionChanged)
    Q_PROPERTY(QString imageName READ imageName NOTIFY imageNameChanged)
    Q_PROPERTY(Type itemType READ itemType CONSTANT)
    Q_PROPERTY(QString resourcePath READ resourcePath CONSTANT)
    Q_PROPERTY(QString itemTypeName READ itemTypeName CONSTANT)
    Q_PROPERTY(QString itemId READ itemId CONSTANT)
    Q_PROPERTY(int price READ price NOTIFY priceChanged)
    Q_PROPERTY(bool playerFocus READ playerFocus NOTIFY playerFocusChanged)
    Q_PROPERTY(bool playerVisible READ playerVisible WRITE setPlayerVisible NOTIFY playerVisibleChanged)
    Q_PROPERTY(bool hidingPlayer READ hidingPlayer NOTIFY hidingPlayerChanged)
    Q_PROPERTY(bool playerOnItem READ playerOnItem WRITE setPlayerOnItem NOTIFY playerOnItemChanged)

    Q_ENUMS(Type)

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
        InteractionPlunder,
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

    QString itemId() const;
    void setItemId(const QString &itemId);

    QString resourcePath() const;
    void setResourcePath(const QString &resourcePath);

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
    void setInteraction(Interaction interaction);

    QString interactionString() const;

    virtual void performInteraction() = 0;

    static QString interactionToString(const Interaction &interaction);

private:
    QString m_itemId;
    QString m_imageName = "/images/game/placeholder.png";
    QString m_resourcePath;
    int m_price = 0;
    bool m_playerFocus = false;
    bool m_playerVisible = false;
    bool m_hidingPlayer = false;
    bool m_playerOnItem = false;

protected:
    Interaction m_interaction = InteractionNone;

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
