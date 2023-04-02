#ifndef GLOBALS_H
#define GLOBALS_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QHostInfo>
#include <QDataStream>
#include <QNetworkInterface>

struct Device
{
    QString name = "NA";
    QString address = "NA";

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

/**
 * @brief get device name and address
 * @return struct Device
 */
Device getDevice();

#endif // GLOBALS_H