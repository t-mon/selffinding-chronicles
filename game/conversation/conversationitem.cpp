#include "conversationitem.h"

ConversationItem::ConversationItem(QObject *parent) : QObject(parent)
{

}

QUuid ConversationItem::uuid() const
{
    return m_uuid;
}

void ConversationItem::setUuid(const QUuid &uuid)
{
    m_uuid = uuid;
}

ConversationItem::Type ConversationItem::type() const
{
    return m_type;
}

void ConversationItem::setType(ConversationItem::Type type)
{
    m_type = type;
}

ConversationItem::Speeker ConversationItem::speeker() const
{
    return m_speeker;
}

void ConversationItem::setSpeeker(ConversationItem::Speeker speeker)
{
    m_speeker = speeker;
}

ConversationItem::Action ConversationItem::action() const
{
    return m_action;
}

void ConversationItem::setAction(ConversationItem::Action action)
{
    m_action = action;
}

QString ConversationItem::text() const
{
    return m_text;
}

void ConversationItem::setText(const QString &text)
{
    m_text = text;
}

QList<ConversationItem *> ConversationItem::choises() const
{
    return m_choises;
}

void ConversationItem::setChoises(const QList<ConversationItem *> &choises)
{
    m_choises = choises;
}

QUuid ConversationItem::nextItemUuid() const
{
    return m_nextItemUuid;
}

void ConversationItem::setNextItemUuid(const QUuid &nextItemUuid)
{
    m_nextItemUuid = nextItemUuid;
}

ConversationItem *ConversationItem::nextItem() const
{
    return m_nextItem;
}

void ConversationItem::setNextItem(ConversationItem *nextItem)
{
    m_nextItem = nextItem;
}
