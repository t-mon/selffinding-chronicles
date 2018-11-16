#include "conversationitem.h"
#include "../debugcategories.h"

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

QString ConversationItem::visibleText() const
{
    return m_visibleText;
}

void ConversationItem::setVisibleText(const QString &text)
{
    if (m_visibleText == text)
        return;

    qCDebug(dcConversation()) << "Visible text" << text;
    m_visibleText = text;
    emit visibleTextChanged(m_visibleText);
}

ConversationItems *ConversationItem::choises() const
{
    return m_choises;
}

void ConversationItem::setChoises(ConversationItems *choises)
{
    m_choises = choises;
}

int ConversationItem::currentChoiseIndex() const
{
    return m_currentChoiseIndex;
}

void ConversationItem::setCurrentChoiseIndex(int currentChoiseIndex)
{
    if (m_currentChoiseIndex == currentChoiseIndex)
        return;

    qCDebug(dcConversation()) << "Current choise index changed" << currentChoiseIndex;
    m_currentChoiseIndex = currentChoiseIndex;
    emit currentChoiseIndexChanged(m_currentChoiseIndex);
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

void ConversationItem::resetItem()
{
    setVisibleText(QString());
}

QDebug operator<<(QDebug debug, ConversationItem *conversationItem)
{
    debug.nospace() << "ConversationItem(" << conversationItem->uuid().toString();
    debug.nospace() << ", " << conversationItem->type();
    debug.nospace() << ", " << conversationItem->speeker();
    debug.nospace() << ") ";
    return debug;
}
