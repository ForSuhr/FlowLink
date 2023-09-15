#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include <QFile>

#include "NetworkGlobals.h"

class TcpSender : public QObject
{
    Q_OBJECT

public:
    explicit TcpSender(const QString &ipv4Address, int port, QObject *parent = nullptr);
    ~TcpSender();

    void sendMsg(const QString &msg);
    void sendBin(const QString &filePath);

private:
    QTcpSocket *tcpSocketIPv4ForMsg = nullptr;
    QTcpSocket *tcpSocketIPv4ForBin = nullptr;
};

#endif