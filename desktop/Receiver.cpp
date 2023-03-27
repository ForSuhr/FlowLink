#include "receiver.h"

Receiver::Receiver(QObject *parent)
    : groupAddressIPv4(QStringLiteral("224.0.0.0"))
{
    udpSocketIPv4.bind(QHostAddress::AnyIPv4, 8080, QUdpSocket::ShareAddress);
}

Receiver::~Receiver()
{
}

void Receiver::createConnection()
{
    udpSocketIPv4.joinMulticastGroup(groupAddressIPv4);

    // get host name and address
    host.name = hostInfo.localHostName();
    hostInfo = QHostInfo::fromName(host.name);
    host.address = "";
    foreach (QHostAddress address, hostInfo.addresses())
    {
        if (address.protocol() == QAbstractSocket::IPv4Protocol)
            host.address = address.toString();
    }

    // send the host info back to UI
    emit sendHostInfo(host);

    // handle incoming packets
    connect(&udpSocketIPv4, &QUdpSocket::readyRead, this, &Receiver::processPendingDatagrams);
}

void Receiver::closeConnection()
{
    udpSocketIPv4.leaveMulticastGroup(groupAddressIPv4);
    udpSocketIPv4.close();
}

void Receiver::processPendingDatagrams()
{
    QByteArray datagram;

    while (udpSocketIPv4.hasPendingDatagrams())
    {
        datagram.resize(int(udpSocketIPv4.pendingDatagramSize()));
        udpSocketIPv4.readDatagram(datagram.data(), datagram.size());
    }
}
