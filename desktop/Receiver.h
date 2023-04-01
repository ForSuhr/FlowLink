#ifndef RECEIVER_H
#define RECEIVER_H

#include "utilities/globals.h"

enum DeviceAction
{
    Connection,
    Disconnection
};

class Receiver : public QObject
{
    Q_OBJECT

public:
    explicit Receiver(QObject *parent = nullptr);
    ~Receiver();
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

#endif // RECEIVER_H
