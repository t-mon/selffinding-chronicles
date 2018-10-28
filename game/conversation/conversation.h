#ifndef CONVERSATION_H
#define CONVERSATION_H

#include <QHash>
#include <QObject>
#include <QString>
#include <QVariantMap>

#include "conversationitem.h"

class Conversation : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString title READ title CONSTANT)
    Q_PROPERTY(QUuid uuid READ uuid CONSTANT)

public:
    explicit Conversation(const QVariantMap conversationMap, QObject *parent = nullptr);

    QString title() const;
    QUuid uuid() const;

    Q_INVOKABLE void selectItem(const QUuid &itemUuid);

private:
    QVariantMap m_conversationMap;
    QString m_title;
    QUuid m_uuid;
    QUuid m_startItemUuid;

    QHash<QUuid, ConversationItem *> m_itemsHash;

    void loadConversation();
    ConversationItem *createConversationItem(const QVariantMap &conversationItemMap);

signals:

public slots:

};

#endif // CONVERSATION_H
