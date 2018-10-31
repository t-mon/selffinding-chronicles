#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QHash>
#include <QTimer>
#include <QObject>
#include <QString>
#include <QVariantMap>

#include "conversationitem.h"
#include "../items/character.h"

class Conversation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QUuid uuid READ uuid CONSTANT)
    Q_PROPERTY(Character *character READ character CONSTANT)
    Q_PROPERTY(ConversationItem *conversationItem READ conversationItem NOTIFY conversationItemChanged)

public:
    explicit Conversation(Character *character, const QVariantMap conversationMap, QObject *parent = nullptr);

    QString title() const;
    QUuid uuid() const;

    Character *character() const;
    ConversationItem *conversationItem() const;

    Q_INVOKABLE void selectItem(const QUuid &itemUuid);

private:
    QTimer *m_timer = nullptr;
    Character *m_character = nullptr;
    QVariantMap m_conversationMap;
    QString m_title;
    QUuid m_uuid;
    QUuid m_startItemUuid;
    int m_timerIntervall = 50;
    int m_timerAnimationCount = 0;
    bool m_finished = false;
    QHash<QUuid, ConversationItem *> m_itemsHash;
    ConversationItem *m_conversationItem = nullptr;

    void setConversationItem(ConversationItem *conversationItem);

    void loadConversation();

    ConversationItem *createConversationItem(const QVariantMap &conversationItemMap);

    void performAction(ConversationItem::Action action);

signals:
    void conversationFinished();
    void conversationItemChanged(ConversationItem *conversationItem);

private slots:
    void onTimeout();

public slots:
    void confirmPressed();


};

#endif // CONVERSATION_H
