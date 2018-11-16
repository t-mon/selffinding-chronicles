#ifndef CONVERSATIONITEMS_H
#define CONVERSATIONITEMS_H

#include <QList>
#include <QAbstractListModel>

class ConversationItem;

class ConversationItems : public QAbstractListModel
{
    Q_OBJECT

public:    
    enum ConversationItemRole {
        TextRole = Qt::DisplayRole,
        VisibleTextRole,
        UuidRole
    };
    Q_ENUM(ConversationItemRole)

    explicit ConversationItems(QObject *parent = nullptr);

    QList<ConversationItem *> conversationItems() const;
    Q_INVOKABLE ConversationItem *get(int index);
    Q_INVOKABLE int count() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const override;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const override;

    void addConversationItem(ConversationItem *item);
    void addConversationItemList(QList<ConversationItem *> itemList);
    void removeConversationItem(ConversationItem *item);

    void clearModel();

protected:
    QHash<int, QByteArray> roleNames() const override;

private:
    QList<ConversationItem *> m_conversationItems;

};

#endif // CONVERSATIONITEMS_H
