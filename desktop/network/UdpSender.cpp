#include <QHostInfo>
#include <QHostAddress>

#include "UdpSender.h"

UdpSender::UdpSender(QObject *parent)
    : groupAddressIPv4(QStringLiteral("224.0.0.1"))
{
    udpSocketIPv4.bind(QHostAddress::AnyIPv4, 8080, QUdpSocket::ShareAddress);
    // set link-local as multicast scope when the TTL of the IPv4 socket is set to 1
    // multicast packet is sent to the multicast group address in the range 224.0.0.0/24
    udpSocketIPv4.setSocketOption(QAbstractSocket::MulticastTtlOption, 1);
}

void UdpSender::sendDeviceInfo(int port)
{
    Device device = localHostName();
    device.port = port;
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::ReadWrite);
    stream << device;
    udpSocketIPv4.writeDatagram(datagram, groupAddressIPv4, 8080);
}

void UdpSender::sendDeviceInfoToLeave(int port)
{
    Device device = localHostName();
    device.port = port;
    device.leaveTheGroup = true;
    QByteArray datagram;
    QDataStream stream(&datagram, QIODevice::ReadWrite);
    stream << device;
    udpSocketIPv4.writeDatagram(datagram, groupAddressIPv4, 8080);
}

void UdpSender::closeConnection()
{
    udpSocketIPv4.close();
}
