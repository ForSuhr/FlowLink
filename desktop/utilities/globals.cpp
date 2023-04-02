#include "globals.h"

Device getDevice()
{
    Device device;
    QHostInfo hostInfo;
    device.name = hostInfo.localHostName();

    foreach (QNetworkInterface interface, QNetworkInterface::allInterfaces())
    {
        if (interface.flags().testFlag(QNetworkInterface::IsLoopBack))
        {
            continue;
        }

        foreach (QNetworkAddressEntry entry, interface.addressEntries())
        {
            if (entry.ip().protocol() == QAbstractSocket::IPv4Protocol)
            {
                device.address = entry.ip().toString();
            }
        }
    }

    return device;
}