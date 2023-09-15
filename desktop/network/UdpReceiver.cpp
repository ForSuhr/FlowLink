#include "UdpReceiver.h"

UdpReceiver::UdpReceiver(QObject *parent)
    : m_groupAddressIPv4(QStringLiteral("224.0.0.1"))
{
    m_udpSocketIPv4.bind(QHostAddress::AnyIPv4, 8080, QUdpSocket::ShareAddress);
    m_udpSocketIPv4.joinMulticastGroup(m_groupAddressIPv4);

    // handle incoming packets
    connect(&m_udpSocketIPv4, &QUdpSocket::readyRead, this, &UdpReceiver::processPendingDatagrams);
}

UdpReceiver::~UdpReceiver()
{
    m_udpSocketIPv4.close();
}

void UdpReceiver::createConnection()
{
    m_udpSocketIPv4.joinMulticastGroup(m_groupAddressIPv4);
}

void UdpReceiver::closeConnection()
{
    m_udpSocketIPv4.leaveMulticastGroup(m_groupAddressIPv4);

    emit receivedDeviceInfo(m_device, DeviceAction::Disconnection);
}

void UdpReceiver::processPendingDatagrams()
{
    QByteArray datagram;
    QHostAddress senderIp;

    while (m_udpSocketIPv4.hasPendingDatagrams())
    {
        datagram.resize(int(m_udpSocketIPv4.pendingDatagramSize()));
        m_udpSocketIPv4.readDatagram(datagram.data(), datagram.size(), &senderIp);
    }

    QDataStream stream(&datagram, QIODevice::ReadOnly);
    stream >> m_device;
    m_device.address = senderIp.toString();
    PLOG_DEBUG << m_device.port;

    if (m_device.name != localHostName().name)
        emit receivedDeviceInfo(m_device, DeviceAction::Connection);
    else // delete the whole "else" section, if you want to filter the local host
        emit receivedDeviceInfo(m_device, DeviceAction::LocalHostConnection);
}
