#include "fields.h"

Fields::Fields(QObject *parent) : QAbstractListModel(parent)
{

}

QList<Field *> Fields::fields() const
{
    return m_fields;
}

int Fields::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_fields.count();
}

QVariant Fields::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_fields.count())
        return QVariant();

    Field *field= m_fields.at(index.row());
    if (role == PositionRole) {
        return field->position();
    } else if (role == AccessibleRole) {
        return field->accessible();
    }

    return QVariant();
}

void Fields::addField(Field *field)
{
    beginInsertRows(QModelIndex(), m_fields.count(), m_fields.count());
    m_fields.append(field);
    endInsertRows();

    QModelIndex i = index(m_fields.indexOf(field));
    emit dataChanged(i, i);
}

QHash<int, QByteArray> Fields::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[AccessibleRole] = "accessible";
    return roles;
}
