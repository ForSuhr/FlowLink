#ifndef GLOBALS_H
#define GLOBALS_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QHostInfo>

struct Device
{
    QString name = "NA";
    QString address = "NA";

    bool operator==(const Device &other) const
    {
        return name == other.name && address == other.address;
    }
};

/**
 * @brief get device name and address
 * @return struct Device
 */
Device getDevice();

#endif // GLOBALS_H