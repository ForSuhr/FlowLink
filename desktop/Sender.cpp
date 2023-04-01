#include <QHostInfo>
#include <QHostAddress>

#include "sender.h"

Sender::Sender(QObject *parent)
    : groupAddressIPv4(QStringLiteral("224.0.0.0"))
{
    udpSocketIPv4.bind(QHostAddress("0.0.0.0"));

    // set link-local as multicast scope when the TTL of the IPv4 socket is set to 1
    // multicast packet is sent to the multicast group address in the range 224.0.0.0/24
    udpSocketIPv4.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
}

void Sender::sendDatagram()
{
    device = getDevice();

    QByteArray datagram = "Guest";
    udpSocketIPv4.writeDatagram(datagram, groupAddressIPv4, 8080);
}

void Sender::closeConnection()
{
    udpSocketIPv4.close();
}
