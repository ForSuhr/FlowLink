#ifndef SENDER_H
#define SENDER_H

#include "utilities/globals.h"

class Sender : public QObject
{
    Q_OBJECT

public:
    explicit Sender(QObject *parent = nullptr);
    void sendDatagram();
    void closeConnection();

private:
    Device device;
    QHostAddress groupAddressIPv4;
    QUdpSocket udpSocketIPv4;
};

#endif