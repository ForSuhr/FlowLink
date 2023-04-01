#include "globals.h"

Device getDevice()
{
    Device device;
    QHostInfo hostInfo;
    device.name = hostInfo.localHostName();
    hostInfo = QHostInfo::fromName(device.name);
    foreach (QHostAddress address, hostInfo.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            device.address = address.toString();
    }
    return device;
}