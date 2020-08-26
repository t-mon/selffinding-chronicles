#ifndef PASSAGEITEM_H
#define PASSAGEITEM_H

#include <QObject>

#include "gameitem.h"

class PassageItem : public GameItem
{
    Q_OBJECT
    Q_PROPERTY(bool enabled READ enabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QString targetMap READ targetMap WRITE setTargetMap NOTIFY targetMapChanged)
    Q_PROPERTY(QPointF targetPosition READ targetPosition WRITE setTargetPosition NOTIFY targetPositionChanged)

public:
    explicit PassageItem(QObject *parent = nullptr);
    ~PassageItem() override = default;

    QString itemTypeName() const override;
    Type itemType() const override;

    Q_INVOKABLE void performInteraction() override;

    bool enabled() const;
    void setEnabled(bool enabled);

    QString targetMap() const;
    void setTargetMap(const QString &targetMap);

    QPointF targetPosition() const;
    void setTargetPosition(const QPointF &targetPosition);

private:
    bool m_enabled = true;
    QString m_targetMap;
    QPointF m_targetPosition;

signals:
    void enabledChanged(bool enabled);
    void targetMapChanged(const QString &targetMap);
    void targetPositionChanged(const QPointF &targetPosition);

};

QDebug operator<<(QDebug debug, PassageItem *passageItem);

#endif // PASSAGEITEM_H
