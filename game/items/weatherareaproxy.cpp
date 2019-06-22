#include "weatherareaproxy.h"
#include "../debugcategories.h"

WeatherAreaModel::WeatherAreaModel(QObject *parent) :
    QAbstractListModel(parent)
{
}

QList<QRect> WeatherAreaModel::areas() const
{
    return m_areas;
}

QSize WeatherAreaModel::areaSize() const
{
    return m_areaSize;
}

QRect WeatherAreaModel::get(int index) const
{
    if (index >= m_areas.count() || index < 0)
        return QRect();

    return m_areas.at(index);
}

int WeatherAreaModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_areas.count();
}

QVariant WeatherAreaModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_areas.count())
        return QVariant();

    QRect area = m_areas.at(index.row());
    if (!area.isValid())
        return QVariant();

    if (role == PositionRole) {
        return QPointF(area.x(), area.y());
    } else if (role == SizeRole) {
        return QSize(area.width(), area.width());
    }

    return QVariant();
}

void WeatherAreaModel::initializeAreas(const QSize &worldSize)
{
    // Create all areas for the whole world, only areas within the viewWindow will be drawn
    beginResetModel();
    m_areas.clear();
    int xCount = worldSize.width() / m_areaSize.width();
    int yCount = worldSize.height() /m_areaSize.height();

    for (int x = 0; x < xCount; x++) {
        for (int y = 0; y < yCount; y++) {
            m_areas.append(QRect(x * m_areaSize.width(), y * m_areaSize.height(), m_areaSize.width(), m_areaSize.height()));
        }
    }

    endResetModel();
    emit countChanged(m_areas.count());
}

QHash<int, QByteArray> WeatherAreaModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[PositionRole] = "position";
    roles[SizeRole] = "size";
    return roles;
}

WeatherAreaProxy::WeatherAreaProxy(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    connect(this, &WeatherAreaProxy::rowsInserted, this, &WeatherAreaProxy::countChanged);
    connect(this, &WeatherAreaProxy::rowsRemoved, this, &WeatherAreaProxy::countChanged);
}

WeatherAreaModel *WeatherAreaProxy::areaModel() const
{
    return m_areaModel;
}

void WeatherAreaProxy::setAreaModel(WeatherAreaModel *areaModel)
{
    if (m_areaModel == areaModel)
        return;

    // Disconnect old model
    if (m_areaModel)
        disconnect(m_areaModel, &WeatherAreaModel::countChanged, this, &WeatherAreaProxy::onSourceModelCountChanged);

    beginResetModel();
    m_areaModel = areaModel;
    emit areaModelChanged(m_areaModel);
    setSourceModel(m_areaModel);
    endResetModel();

    if (m_areaModel) {
        connect(m_areaModel, &WeatherAreaModel::countChanged, this, &WeatherAreaProxy::onSourceModelCountChanged);
        invalidateFilter();
    }

    emit countChanged();
}

QSize WeatherAreaProxy::areaSize() const
{
    return m_areaModel->areaSize();
}

int WeatherAreaProxy::count() const
{
    return rowCount(QModelIndex());
}

QRectF WeatherAreaProxy::viewFilter() const
{
    return m_viewFilter;
}

void WeatherAreaProxy::setViewFilter(const QRectF &viewFilter)
{
    if (m_viewFilter == viewFilter)
        return;

    m_viewFilter = viewFilter;
    invalidateFilter();
    emit countChanged();
}

bool WeatherAreaProxy::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    Q_UNUSED(sourceParent)

    // Get the current game item
    if (!m_areaModel)
        return true;

    QRect area = m_areaModel->get(sourceRow);
    if (!area.isValid())
        return true;

    // Filter out items not in the view filter
    if (!m_viewFilter.isNull() && !m_viewFilter.intersects(area))
        return false;

    return true;
}

void WeatherAreaProxy::onSourceModelCountChanged()
{
    invalidateFilter();
}
