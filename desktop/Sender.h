#ifndef SENDER_H
#define SENDER_H

#include <QHostAddress>
#include <QUdpSocket>

class Sender : public QObject
{
    Q_OBJECT

public:
    explicit Sender(QObject *parent = nullptr);
    void sendDatagram();
    void closeConnection();

private:
    QUdpSocket udpSocketIPv4;
    QHostAddress groupAddressIPv4;
};

#endif