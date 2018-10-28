#ifndef CONVERSATIONITEM_H
#define CONVERSATIONITEM_H

#include <QUuid>
#include <QObject>

class ConversationItem : public QObject
{
    Q_OBJECT
public:
    enum Type {
        TypeText,
        TypeChoise,
        TypeAction
    };
    Q_ENUM(Type)

    enum Speeker {
        SpeekerPlayer,
        SpeekerCharacter
    };
    Q_ENUM(Speeker)

    enum Action {
        ActionNone,
        ActionEndConversation,
        ActionDeal
    };
    Q_ENUM(Action)

    explicit ConversationItem(QObject *parent = nullptr);

    QUuid uuid() const;
    void setUuid(const QUuid &uuid);

    Type type() const;
    void setType(Type type);

    Speeker speeker() const;
    void setSpeeker(Speeker speeker);

    Action action() const;
    void setAction(Action action);

    QString text() const;
    void setText(const QString &text);

    QList<ConversationItem *> choises() const;
    void setChoises(const QList<ConversationItem *> &choises);

    QUuid nextItemUuid() const;
    void setNextItemUuid(const QUuid &nextItemUuid);

    ConversationItem *nextItem() const;
    void setNextItem(ConversationItem *nextItem);

private:
    QUuid m_uuid;
    Type m_type = TypeText;
    Speeker m_speeker = SpeekerPlayer;
    Action m_action = ActionNone;
    QString m_text;
    QList<ConversationItem *> m_choises;
    QUuid m_nextItemUuid;
    ConversationItem *m_nextItem = nullptr;

};

#endif // CONVERSATIONITEM_H
