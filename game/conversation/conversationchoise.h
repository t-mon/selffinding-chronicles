#ifndef CONVERSATIONCHOISE_H
#define CONVERSATIONCHOISE_H

#include <QObject>

#include "conversationitem.h"

class ConversationChoise : public ConversationItem
{
    Q_OBJECT
public:
    explicit ConversationChoise(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CONVERSATIONCHOISE_H
