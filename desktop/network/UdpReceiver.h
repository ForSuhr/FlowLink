#ifndef UDPRECEIVER_H
#define UDPRECEIVER_H

#include "NetworkGlobals.h"

enum DeviceAction
{
    Connection,
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
    void sendDeviceInfo(Device device, DeviceAction deviceAction);

private slots:
    void processPendingDatagrams();

private:
    Device device;
    QHostInfo hostInfo;
    QUdpSocket udpSocketIPv4;
    QHostAddress groupAddressIPv4;
};

#endif
