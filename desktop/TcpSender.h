#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include "Globals.h"

class TcpSender : public QObject
{
    Q_OBJECT

public:
    explicit TcpSender(const QString &ipv4Address, QObject *parent = nullptr);
    ~TcpSender();

    void sendMsg(const QString &msg);

private:
    QTcpSocket *tcpSocketIPv4;
};

#endif