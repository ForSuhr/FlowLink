#ifndef NETWORK_GLOBALS_H
#define NETWORK_GLOBALS_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostInfo>
#include <QDataStream>
#include <QNetworkInterface>
#include <QCborStreamWriter>
#include <QCborStreamReader>

struct Device
{
    QString name = "NA";
    QString address = "NA";

    bool operator==(const Device &other) const
    {
        return name == other.name && address == other.address;
    }
};

enum DataType
{
    PlainText
};

inline QDataStream &operator<<(QDataStream &stream, const Device &device)
{
    return stream << device.name << device.address;
}

inline QDataStream &operator>>(QDataStream &stream, Device &device)
{
    return stream >> device.name >> device.address;
}

/**
 * @brief get device name and address
 * @return struct Device
 */
Device getLocalHostName();

#endif