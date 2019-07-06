#ifndef TELEPORTERITEM_H
#define TELEPORTERITEM_H

#include <QDebug>
#include <QObject>

#include "gameitem.h"

class TeleporterItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(QString targetMap READ targetMap WRITE setTargetMap NOTIFY targetMapChanged)
    Q_PROPERTY(QPointF targetPosition READ targetPosition WRITE setTargetPosition NOTIFY targetPositionChanged)

public:
    explicit TeleporterItem(QObject *parent = nullptr);
    ~TeleporterItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    QString targetMap() const;
    void setTargetMap(const QString &targetMap);

    QPointF targetPosition() const;
    void setTargetPosition(const QPointF &targetPosition);

private:
    QString m_targetMap;
    QPointF m_targetPosition;

signals:
    void targetMapChanged(const QString &targetMap);
    void targetPositionChanged(const QPointF &targetPosition);

};

QDebug operator<<(QDebug debug, TeleporterItem *teleporterItem);


#endif // TELEPORTERITEM_H
