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
        AccessibleRole,
        ImageNameRole,
        PlayerOnFieldRole,
        InPlayerRangeRole
    };
    Q_ENUM(FieldRole)

    explicit Fields(QObject *parent = nullptr);
    ~Fields() = default;

    QList<Field *> fields() const;
    Q_INVOKABLE Field *get(int index);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void addField(Field *field);
    void removeField(Field *field);

    void clearModel();

protected:
    QHash<int, QByteArray> roleNames() const;

private:
    QList<Field *> m_fields;

private slots:
    void onFieldAccessableChanged(bool accessable);
    void onFieldImageNameChanged(const QString &imageName);
    void onPlayerOnFieldChanged(bool playerOnField);
    void onFieldInPlayerRangeChanged(bool inPlayerRange);


};

#endif // FIELDS_H
