#include "Receiver.h"

Receiver::Receiver(QObject *parent)
    : groupAddressIPv4(QStringLiteral("224.0.0.1"))
{
    udpSocketIPv4.bind(QHostAddress::AnyIPv4, 8080, QUdpSocket::ShareAddress);
    udpSocketIPv4.joinMulticastGroup(groupAddressIPv4);

    // handle incoming packets
    connect(&udpSocketIPv4, &QUdpSocket::readyRead, this, &Receiver::processPendingDatagrams);
}

Receiver::~Receiver()
{
    udpSocketIPv4.close();
}

void Receiver::createConnection()
{
    udpSocketIPv4.joinMulticastGroup(groupAddressIPv4);
}

void Receiver::closeConnection()
{
    udpSocketIPv4.leaveMulticastGroup(groupAddressIPv4);

    emit sendDeviceInfo(device, DeviceAction::Disconnection);
}

void Receiver::processPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocketIPv4.hasPendingDatagrams())
    {
        datagram.resize(int(udpSocketIPv4.pendingDatagramSize()));
        udpSocketIPv4.readDatagram(datagram.data(), datagram.size());
    }

    QDataStream stream(&datagram, QIODevice::ReadOnly);
    stream >> device;

    if (device.name == getLocalHostName().name)
    {
        return;
    }

    hostInfo = QHostInfo::fromName(device.name);

    if (!hostInfo.addresses().isEmpty())
    {
        foreach (QHostAddress address, hostInfo.addresses())
        {
            if (address.protocol() == QAbstractSocket::IPv4Protocol)
            {
                device.address = address.toString();
            }
        }
    }

    emit sendDeviceInfo(device, DeviceAction::Connection);
}
