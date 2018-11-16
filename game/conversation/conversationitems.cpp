#include "conversationitems.h"
#include "conversationitem.h"

ConversationItems::ConversationItems(QObject *parent)
    : QAbstractListModel(parent)
{
}

QList<ConversationItem *> ConversationItems::conversationItems() const
{
    return m_conversationItems;
}

ConversationItem *ConversationItems::get(int index)
{
    if (index >= m_conversationItems.count() || index < 0)
        return nullptr;

    return m_conversationItems.at(index);
}

int ConversationItems::count() const
{
    return m_conversationItems.count();
}

int ConversationItems::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_conversationItems.count();
}

QVariant ConversationItems::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_conversationItems.count())
        return QVariant();

    ConversationItem *conversationItem = m_conversationItems.at(index.row());
    if (!conversationItem)
        return QVariant();

    if (role == TextRole) {
        return conversationItem->text();
    } else if (role == VisibleTextRole) {
        return conversationItem->visibleText();
    } else if (role == UuidRole) {
        return conversationItem->uuid();
    }

    return QVariant();
}

void ConversationItems::addConversationItem(ConversationItem *item)
{
    if (m_conversationItems.contains(item))
        return;

    beginInsertRows(QModelIndex(), m_conversationItems.count(), m_conversationItems.count());
    m_conversationItems.append(item);
    endInsertRows();
}

void ConversationItems::addConversationItemList(QList<ConversationItem *> itemList)
{
    foreach (ConversationItem *item, itemList) {
        addConversationItem(item);
    }
}

void ConversationItems::removeConversationItem(ConversationItem *item)
{
    if (!m_conversationItems.contains(item))
        return;

    int index = m_conversationItems.indexOf(item);
    beginRemoveRows(QModelIndex(), index, index);
    m_conversationItems.removeAll(item);
    endRemoveRows();
}

void ConversationItems::clearModel()
{
    beginResetModel();
    m_conversationItems.clear();
    endResetModel();
}

QHash<int, QByteArray> ConversationItems::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TextRole] = "text";
    roles[VisibleTextRole] = "visibleText";
    roles[UuidRole] = "uuid";
    return roles;
}
