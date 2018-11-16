#ifndef CONVERSATIONACTION_H
#define CONVERSATIONACTION_H

#include <QObject>

class ConversationAction : public QObject
{
    Q_OBJECT
public:
    explicit ConversationAction(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CONVERSATIONACTION_H