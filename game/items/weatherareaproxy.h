#ifndef WEATHERAREAPROXY_H
#define WEATHERAREAPROXY_H

#include <QRect>
#include <QObject>
#include <QAbstractListModel>
#include <QSortFilterProxyModel>

class WeatherAreaModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum Role {
        PositionRole,
        SizeRole
    };
    Q_ENUM(Role)

    explicit WeatherAreaModel(QObject *parent = nullptr);

    QList<QRect> areas() const;
    QSize areaSize() const;

    Q_INVOKABLE QRect get(int index) const;

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    void initializeAreas(const QSize &worldSize);

private:
    QList<QRect> m_areas;
    QSize m_areaSize = QSize(20, 20);

protected:
    QHash<int, QByteArray> roleNames() const;

signals:
    void countChanged(int count);

};


class WeatherAreaProxy : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
    Q_PROPERTY(QSize areaSize READ areaSize CONSTANT)

public:
    explicit WeatherAreaProxy(QObject *parent = nullptr);

    WeatherAreaModel *areaModel() const;
    void setAreaModel(WeatherAreaModel *areaModel);

    QSize areaSize() const;

    int count() const;

    Q_INVOKABLE void resetFilter();

    QRectF viewFilter() const;
    void setViewFilter(const QRectF &viewFilter);

private:
    WeatherAreaModel *m_areaModel = nullptr;

    QRectF m_viewFilter = QRectF();

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

signals:
    void areaModelChanged(WeatherAreaModel *areaModel);
    void countChanged();

private slots:
    void onSourceModelCountChanged();

};

#endif // WEATHERAREAPROXY_H
