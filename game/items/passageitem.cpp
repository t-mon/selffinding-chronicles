#include "passageitem.h"
#include "../debugcategories.h"

PassageItem::PassageItem(QObject *parent) :
    GameItem(parent)
{

}

QString PassageItem::itemTypeName() const
{
    return tr("Passage");
}

GameItem::Type PassageItem::itemType() const
{
    return GameItem::TypePassageItem;
}

void PassageItem::performInteraction()
{

}

bool PassageItem::enabled() const
{
    return m_enabled;
}

void PassageItem::setEnabled(bool enabled)
{
    if (m_enabled == enabled)
        return;

    m_enabled = enabled;
    emit enabledChanged(m_enabled);
}

QString PassageItem::targetMap() const
{
    return m_targetMap;
}

void PassageItem::setTargetMap(const QString &targetMap)
{
    if (m_targetMap == targetMap)
        return;

    m_targetMap = targetMap;
    emit targetMapChanged(m_targetMap);
}

QPointF PassageItem::targetPosition() const
{
    return m_targetPosition;
}

void PassageItem::setTargetPosition(const QPointF &targetPosition)
{
    if (m_targetPosition == targetPosition)
        return;

    m_targetPosition = targetPosition;
    emit targetPositionChanged(m_targetPosition);
}

QDebug operator<<(QDebug debug, PassageItem *passageItem)
{
    debug.nospace() << "PassageItem(" << passageItem->name();
    debug.nospace() << ", " << (passageItem->enabled() ? "enbled" : "disabled");
    debug.nospace() << ", " << passageItem->targetMap();
    debug.nospace() << ", " << passageItem->targetPosition();
    debug.nospace() << ")";
    return debug.space();
}
