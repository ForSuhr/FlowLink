#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>

struct Device
{
    QString name;
    QString address;

    bool operator==(const Device &other) const
    {
        return name == other.name && address == other.address;
    }
};

inline QDataStream &operator<<(QDataStream &stream, const Device &device)
{
    return stream << device.name << device.address;
}

inline QDataStream &operator>>(QDataStream &stream, Device &device)
{
    return stream >> device.name >> device.address;
}

class TableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    TableModel(QObject *parent = nullptr);
    TableModel(const QVector<Device> &devices, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    const QVector<Device> &getDevices() const;

private:
    QVector<Device> devices;
};

#endif // TABLEMODEL_H
