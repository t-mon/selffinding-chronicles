#include "teleporteritem.h"

TeleporterItem::TeleporterItem(QObject *parent) :
    GameItem(parent)
{
    setInteraction(InteractionPick);
    setInventoryInteraction(InteractionUse);
}

QString TeleporterItem::itemTypeName() const
{
    return tr("Teleport item");
}

GameItem::Type TeleporterItem::itemType() const
{
    return Type::TypeTeleportItem;
}

void TeleporterItem::performInteraction()
{

}

QString TeleporterItem::targetMap() const
{
    return m_targetMap;
}

void TeleporterItem::setTargetMap(const QString &targetMap)
{
    if (m_targetMap == targetMap)
        return;

    m_targetMap = targetMap;
    emit targetMapChanged(m_targetMap);
}

QPointF TeleporterItem::targetPosition() const
{
    return m_targetPosition;
}

void TeleporterItem::setTargetPosition(const QPointF &targetPosition)
{
    if (m_targetPosition == targetPosition)
        return;

    m_targetPosition = targetPosition;
    emit targetPositionChanged(m_targetPosition);
}

QDebug operator<<(QDebug debug, TeleporterItem *teleporterItem)
{
    debug.nospace() << "TeleportItem(" << teleporterItem->name();
    debug.nospace() << ", " << teleporterItem->targetMap();
    debug.nospace() << ", " << teleporterItem->targetPosition();
    debug.nospace() << ")";
    return debug.space();
}
