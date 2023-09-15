#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include "NetworkGlobals.h"

enum DeviceAction
{
    Connection,
    LocalHostConnection,
    Disconnection,
};

class UdpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit UdpReceiver(QObject *parent = nullptr);
    ~UdpReceiver();
    void createConnection();
    void closeConnection();

signals:
    void receivedDeviceInfo(Device device, DeviceAction deviceAction);

private:
    Device m_device;
    QHostInfo m_hostInfo;
    QUdpSocket m_udpSocketIPv4;
    QHostAddress m_groupAddressIPv4;

private slots:
    void processPendingDatagrams();
};

#endif
