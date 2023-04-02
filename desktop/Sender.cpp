#include <QHostInfo>
#include <QHostAddress>

#include "Sender.h"

Sender::Sender(QObject *parent)
    : groupAddressIPv4(QStringLiteral("224.0.0.1"))
{
}

void Sender::sendDatagram()
{
    QUdpSocket udpSocketIPv4;
    udpSocketIPv4.bind(QHostAddress::AnyIPv4, 8080, QUdpSocket::ShareAddress);
    // set link-local as multicast scope when the TTL of the IPv4 socket is set to 1
    // multicast packet is sent to the multicast group address in the range 224.0.0.0/24
    udpSocketIPv4.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
    device = getDevice();
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::ReadWrite);
    stream << device;
    udpSocketIPv4.writeDatagram(datagram, groupAddressIPv4, 8080);
    udpSocketIPv4.close();
}
