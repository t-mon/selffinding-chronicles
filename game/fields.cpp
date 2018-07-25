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
    } else if (role == AccessibleRole) {
        return field->accessible();
    } else if (role == ImageNameRole) {
        return field->imageName();
    } else if (role == PlayerOnFieldRole) {
        return field->playerOnField();
    } else if (role == InPlayerRangeRole) {
        return field->inPlayerRange();
    }

    return QVariant();
}

void Fields::addField(Field *field)
{
    beginInsertRows(QModelIndex(), m_fields.count(), m_fields.count());
    m_fields.append(field);
    endInsertRows();

    connect(field, &Field::accessibleChanged, this, &Fields::onFieldAccessableChanged);
    connect(field, &Field::imageNameChanged, this, &Fields::onFieldImageNameChanged);
    connect(field, &Field::playerOnFieldChanged, this, &Fields::onPlayerOnFieldChanged);
    connect(field, &Field::inPlayerRangeChanged, this, &Fields::onFieldInPlayerRangeChanged);
}

void Fields::removeField(Field *field)
{
    if (!m_fields.contains(field))
        return;

    disconnect(field, &Field::accessibleChanged, this, &Fields::onFieldAccessableChanged);
    disconnect(field, &Field::imageNameChanged, this, &Fields::onFieldImageNameChanged);
    disconnect(field, &Field::playerOnFieldChanged, this, &Fields::onPlayerOnFieldChanged);
    disconnect(field, &Field::inPlayerRangeChanged, this, &Fields::onFieldInPlayerRangeChanged);

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
    roles[AccessibleRole] = "accessible";
    roles[ImageNameRole] = "imageName";
    roles[PlayerOnFieldRole] = "playerOnField";
    roles[InPlayerRangeRole] = "inPlayerRange";
    return roles;
}

void Fields::onFieldAccessableChanged(bool accessable)
{
    Q_UNUSED(accessable)

    Field *field = static_cast<Field *>(sender());
    int idx = m_fields.indexOf(field);
    if (idx < 0) return;
    emit dataChanged(index(idx), index(idx), {AccessibleRole});
}

void Fields::onFieldImageNameChanged(const QString &imageName)
{
    Q_UNUSED(imageName)

    Field *field = static_cast<Field *>(sender());
    int idx = m_fields.indexOf(field);
    if (idx < 0) return;
    emit dataChanged(index(idx), index(idx), {ImageNameRole});
}

void Fields::onPlayerOnFieldChanged(bool playerOnField)
{
    Q_UNUSED(playerOnField)

    Field *field = static_cast<Field *>(sender());
    int idx = m_fields.indexOf(field);
    if (idx < 0) return;
    emit dataChanged(index(idx), index(idx), {PlayerOnFieldRole});
}

void Fields::onFieldInPlayerRangeChanged(bool inPlayerRange)
{
    Q_UNUSED(inPlayerRange)

    Field *field = static_cast<Field *>(sender());
    int idx = m_fields.indexOf(field);
    if (idx < 0) return;
    emit dataChanged(index(idx), index(idx), {InPlayerRangeRole});
}
