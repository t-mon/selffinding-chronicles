#ifndef CONVERSATIONITEM_H
#define CONVERSATIONITEM_H

#include <QUuid>
#include <QDebug>
#include <QObject>

class ConversationItems;

class ConversationItem : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUuid uuid READ uuid CONSTANT)
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(Speeker speeker READ speeker CONSTANT)
    Q_PROPERTY(QString text READ text CONSTANT)
    Q_PROPERTY(QString visibleText READ visibleText NOTIFY visibleTextChanged)
    Q_PROPERTY(ConversationItems *choises READ choises CONSTANT)
    Q_PROPERTY(int currentChoiseIndex READ currentChoiseIndex WRITE setCurrentChoiseIndex NOTIFY currentChoiseIndexChanged)

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

    QString visibleText() const;
    void setVisibleText(const QString &text);

    ConversationItems *choises() const;
    void setChoises(ConversationItems *choises);

    int currentChoiseIndex() const;
    void setCurrentChoiseIndex(int currentChoiseIndex);

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
    QString m_visibleText;
    QUuid m_nextItemUuid;
    int m_currentChoiseIndex = 0;
    ConversationItems *m_choises = nullptr;
    ConversationItem *m_nextItem = nullptr;

signals:
    void visibleTextChanged(const QString &text);
    void currentChoiseIndexChanged(int currentChoiseIndex);

public slots:
    void resetItem();

};

QDebug operator<<(QDebug debug, ConversationItem *conversationItem);

#endif // CONVERSATIONITEM_H
