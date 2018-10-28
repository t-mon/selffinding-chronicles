#include "conversation.h"
#include "../debugcategories.h"

Conversation::Conversation(const QVariantMap conversationMap, QObject *parent) :
    QObject(parent),
    m_conversationMap(conversationMap)
{
    loadConversation();
}

QString Conversation::title() const
{
    return m_title;
}

QUuid Conversation::uuid() const
{
    return m_uuid;
}

void Conversation::selectItem(const QUuid &itemUuid)
{

}

void Conversation::loadConversation()
{
    m_title = m_conversationMap.value("title").toString();
    m_uuid = m_conversationMap.value("uuid").toUuid();
    m_startItemUuid = m_conversationMap.value("start").toUuid();
    qCDebug(dcConversation()) << "Loading conversation" << m_title << m_uuid.toString();

    // Load all conversation items
    foreach (const QVariant &conversationVariant, m_conversationMap.value("conversation").toList()) {
        ConversationItem *item = createConversationItem(conversationVariant.toMap());
        m_itemsHash.insert(item->uuid(), item);
    }

    // Link the items

    // Verify conversation consistency

    // Start conversation

    // Set current conversation item

}

ConversationItem *Conversation::createConversationItem(const QVariantMap &conversationItemMap)
{
    ConversationItem *item = new ConversationItem(this);
    item->setUuid(conversationItemMap.value("uuid").toUuid());

    // Load speeker
    QString itemSpeekerString = conversationItemMap.value("speeker").toString().toLower();
    if (itemSpeekerString == "character") {
        item->setSpeeker(ConversationItem::SpeekerCharacter);
    } else {
        item->setSpeeker(ConversationItem::SpeekerPlayer);
    }

    // Load item type
    QString itemTypeString = conversationItemMap.value("type").toString().toLower();
    if (itemTypeString == "choise") {
        item->setType(ConversationItem::TypeChoise);
    } else if (itemTypeString == "action") {
        item->setType(ConversationItem::TypeAction);
    } else {
        item->setType(ConversationItem::TypeText);
    }

    // Load type specific stuff
    switch (item->type()) {
    case ConversationItem::TypeText:
        item->setText(tr(conversationItemMap.value("text").toString().toLatin1().data()));
        item->setNextItemUuid(conversationItemMap.value("next").toUuid());
        break;
    case ConversationItem::TypeChoise:
        item->setText(tr(conversationItemMap.value("text").toString().toLatin1().data()));
        // Load choise items
        foreach (const QVariant &choiseVariant, conversationItemMap.value("choises").toList()) {
            ConversationItem *item = createConversationItem(choiseVariant.toMap());
            m_itemsHash.insert(item->uuid(), item);
        }
        break;
    case ConversationItem::TypeAction:
        item->setText(tr(conversationItemMap.value("text").toString().toLatin1().data()));
        item->setNextItemUuid(conversationItemMap.value("next").toUuid());

        // Load action type
        QString actionTypeString = conversationItemMap.value("action").toString().toLower();
        if (actionTypeString == "endconversation") {
            item->setAction(ConversationItem::ActionEndConversation);
        } else if (actionTypeString == "deal") {
            item->setAction(ConversationItem::ActionDeal);
        } else {
            item->setAction(ConversationItem::ActionNone);
        }

        break;
    }

    return item;
}
