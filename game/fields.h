#ifndef FIELDS_H
#define FIELDS_H

#include <QObject>
#include <QAbstractListModel>

#include "field.h"

class Fields : public QAbstractListModel
{
    Q_OBJECT
public:
    enum FieldRole {
        PositionRole,
        AccessibleRole
    };
    Q_ENUM(FieldRole)

    explicit Fields(QObject *parent = nullptr);

    QList<Field *> fields() const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void addField(Field *field);

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Field *> m_fields;

signals:

};

#endif // FIELDS_H
