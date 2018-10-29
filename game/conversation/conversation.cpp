#include "conversation.h"
#include "conversationitems.h"
#include "../debugcategories.h"

Conversation::Conversation(Character *character, const QVariantMap conversationMap, QObject *parent) :
    QObject(parent),
    m_character(character),
    m_conversationMap(conversationMap)
{
    m_timer = new QTimer(this);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, &Conversation::onTimeout);

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

Character *Conversation::character() const
{
    return m_character;
}

ConversationItem *Conversation::conversationItem() const
{
    return m_conversationItem;
}

void Conversation::selectItem(const QUuid &itemUuid)
{
    qCDebug(dcConversation()) << "Select item" << itemUuid.toString();

    if (!m_itemsHash.contains(itemUuid)) {
        qCWarning(dcConversation()) << "Could not find item for" << m_startItemUuid.toString();
        return;
    }

    setConversationItem(m_itemsHash.value(itemUuid));
}

void Conversation::setConversationItem(ConversationItem *conversationItem)
{
    if (m_conversationItem == conversationItem)
        return;

    qCDebug(dcConversation()) << "Current item changed" << conversationItem;

    if (m_conversationItem)
        m_conversationItem->resetItem();

    m_conversationItem = conversationItem;
    m_conversationItem->resetItem();
    m_timerAnimationCount = 0;
    emit conversationItemChanged(m_conversationItem);

    switch (m_conversationItem->type()) {
    case ConversationItem::TypeText:
        m_timer->start(m_timerIntervall);
        break;
    case ConversationItem::TypeAction:
        performAction(m_conversationItem->action());
        break;
    default:
        break;
    }
}

void Conversation::loadConversation()
{
    m_title = m_conversationMap.value("title").toString();
    m_uuid = m_conversationMap.value("uuid").toUuid();
    qCDebug(dcConversation()) << "Loading conversation" << m_title << m_uuid.toString();

    // Load all conversation items
    foreach (const QVariant &conversationVariant, m_conversationMap.value("conversation").toList()) {
        ConversationItem *item = createConversationItem(conversationVariant.toMap());
        m_itemsHash.insert(item->uuid(), item);
    }

    // Link the items
    foreach (ConversationItem *item, m_itemsHash.values()) {
        ConversationItem *nextItem = m_itemsHash.value(item->nextItemUuid());
        if (nextItem) {
            item->setNextItem(nextItem);
        }
    }

    // TODO: Verify conversation consistency

    // Start conversation
    m_startItemUuid = m_conversationMap.value("start").toUuid();
    ConversationItem *currentConversationItem = m_itemsHash.value(m_startItemUuid);
    if (!currentConversationItem) {
        qCWarning(dcConversation()) << "Could not find conversation start item." << m_title << m_uuid.toString();
        emit conversationFinished();
        return;
    }

    setConversationItem(currentConversationItem);
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
    case ConversationItem::TypeChoise: {
        item->setText(tr(conversationItemMap.value("text").toString().toLatin1().data()));
        // Load choise items
        ConversationItems *items = new ConversationItems(this);
        foreach (const QVariant &choiseVariant, conversationItemMap.value("choises").toList()) {
            ConversationItem *choiseItem = createConversationItem(choiseVariant.toMap());
            items->addConversationItem(choiseItem);
            m_itemsHash.insert(choiseItem->uuid(), choiseItem);
        }
        item->setChoises(items);
        break;
    }
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

    qCDebug(dcConversation()) << "Loaded conversation item" << item;

    return item;
}

void Conversation::performAction(ConversationItem::Action action)
{
    qCDebug(dcConversation()) << "Perform action" << action;

    switch (action) {
    case ConversationItem::ActionEndConversation:
        emit conversationFinished();
        break;
    default:
        break;
    }

}

void Conversation::onTimeout()
{
    QString currentText = m_conversationItem->visibleText().left(m_timerAnimationCount);

    // Check if the text animation is finished
    if (currentText == m_conversationItem->text()) {
        qCDebug(dcConversation()) << "Text animation finished";
        m_timerAnimationCount = m_conversationItem->text().count();
        m_timer->stop();
    } else {
        m_timerAnimationCount++;
        QString visibleText = m_conversationItem->text().left(m_timerAnimationCount);
        qCDebug(dcConversation()) << "Visible text:" << visibleText << m_conversationItem->text().count() - m_timerAnimationCount << "-" << QString("").fill(' ', m_conversationItem->text().count() - m_timerAnimationCount) << "-";
        m_conversationItem->setVisibleText(visibleText + QString("").fill(' ', m_conversationItem->text().count() - m_timerAnimationCount));
    }
}


void Conversation::confirmPressed()
{
    if (m_conversationItem->type() != ConversationItem::TypeText)
        return;

    if (m_conversationItem->visibleText() != m_conversationItem->text()) {
        qCDebug(dcConversation()) << "Confirm pressed: stopping animation";
        m_timer->stop();
        m_conversationItem->setVisibleText(m_conversationItem->text());
    } else {
        qCDebug(dcConversation()) << "Confirm pressed: set next conversation item.";
        if (m_conversationItem->nextItem()) {
            setConversationItem(m_conversationItem->nextItem());
        } else {
            qCWarning(dcConversation()) << "Could not find next conversation item for" << m_conversationItem;
        }
    }
}
