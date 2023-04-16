#ifndef UDPSENDER_H
#define UDPSENDER_H

#include "globals.h"

class UdpSender : public QObject
{
    Q_OBJECT

public:
    explicit UdpSender(QObject *parent = nullptr);
    void sendDeviceInfo();
    void closeConnection();

private:
    QHostAddress groupAddressIPv4;
    QUdpSocket udpSocketIPv4;
};

#endif