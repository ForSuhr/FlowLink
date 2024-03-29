#include "TableModel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

TableModel::TableModel(const QVector<Device> &devices, QObject *parent)
    : QAbstractTableModel(parent),
      m_devices(devices)
{
}

int TableModel::rowCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : m_devices.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    return parent.isValid() ? 0 : 2; // the column count is 2, as there are 2 QString members in struct Device
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= m_devices.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        const Device &device = m_devices.at(index.row());

        switch (index.column())
        {
        case 0:
            return device.name;
        case 1:
            return device.address;
        default:
            break;
        }
    }

    if (role == Qt::TextAlignmentRole)
    {
        switch (index.column())
        {
        case 0:
            return Qt::AlignCenter;
        case 1:
            return Qt::AlignCenter;
        default:
            break;
        }
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case TableHeader::Name:
            return tr("Name");
        case TableHeader::Address:
            return tr("Address");
        default:
            break;
        }
    }
    return QVariant();
}

bool TableModel::insertRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginInsertRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        m_devices.insert(position, {QString(), QString()});

    endInsertRows();
    return true;
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row)
        m_devices.removeAt(position);

    endRemoveRows();
    return true;
}

/**
 * @brief TableModel::setData
 * @param index  the index of the row to set
 * @param value the data to set
 * @param role Qt::ItemDataRole
 * @return true if succeed, otherwise false
 */
bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (index.isValid())
    {
        const int row = index.row();
        Device device = m_devices.value(row);

        switch (index.column())
        {
        case 0:
            device.name = value.toString();
            break;
        case 1:
            device.address = value.toString();
            break;
        default:
            return false;
        }

        m_devices.replace(row, device);
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});

        return true;
    }

    return false;
}

void TableModel::addRow(const QString &name, const QString &address)
{
    if (!m_devices.contains({name, address}))
    {
        insertRows(0, 1, QModelIndex());
        QModelIndex modelIndex = index(0, 0, QModelIndex());
        setData(modelIndex, name, Qt::DisplayRole);
        modelIndex = index(0, 1, QModelIndex());
        setData(modelIndex, address, Qt::DisplayRole);
    }
}

void TableModel::removeRow(const QString &name, const QString &address)
{
    if (m_devices.contains({name, address}))
        removeRows(0, 1, QModelIndex());
}

const QVector<Device> &TableModel::getDevices() const
{
    return m_devices;
}
