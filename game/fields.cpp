#include "fields.h"

Fields::Fields(QObject *parent) : QAbstractListModel(parent)
{

}

QList<Field *> Fields::fields() const
{
    return m_fields;
}

Field *Fields::get(int index)
{
    if (index >= m_fields.count() || index < 0)
        return nullptr;

    return m_fields.at(index);
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

    Field *field = m_fields.at(index.row());
    if (!field)
        return QVariant();

    if (role == PositionRole) {
        return field->position();
    } else if (role == PlayerOnFieldRole) {
        return field->playerOnField();
    }

    return QVariant();
}

void Fields::addField(Field *field)
{
    beginInsertRows(QModelIndex(), m_fields.count(), m_fields.count());
    m_fields.append(field);
    endInsertRows();

    connect(field, &Field::playerOnFieldChanged, this, &Fields::onPlayerOnFieldChanged);
}

void Fields::removeField(Field *field)
{
    if (!m_fields.contains(field))
        return;

    disconnect(field, &Field::playerOnFieldChanged, this, &Fields::onPlayerOnFieldChanged);

    int index = m_fields.indexOf(field);
    beginRemoveRows(QModelIndex(), index, index);
    m_fields.removeOne(field);
    endRemoveRows();
}

void Fields::clearModel()
{
    beginResetModel();
    m_fields.clear();
    endResetModel();
}

QHash<int, QByteArray> Fields::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[PlayerOnFieldRole] = "playerOnField";
    return roles;
}

void Fields::onPlayerOnFieldChanged(bool playerOnField)
{
    Q_UNUSED(playerOnField)

    Field *field = static_cast<Field *>(sender());
    int idx = m_fields.indexOf(field);
    if (idx < 0) return;
    emit dataChanged(index(idx), index(idx), {PlayerOnFieldRole});
}

