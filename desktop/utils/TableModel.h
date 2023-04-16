#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <stdio.h>
#include <QAbstractTableModel>
#include <QVector>
#include <QModelIndex>

#include "globals.h"

enum TableHeader
{
    Name,
    Address
};

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
    bool setData(const QModelIndex &index, const QVariant &value, int role) override;
    bool insertRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;

    void addRow(const QString &name, const QString &address);
    void removeRow(const QString &name, const QString &address);
    const QVector<Device> &getDevices() const;

private:
    QVector<Device> devices;
};

#endif // TABLEMODEL_H
