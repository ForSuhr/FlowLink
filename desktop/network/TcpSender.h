#ifndef TCP_SENDER_H
#define TCP_SENDER_H

#include <QFile>

#include "NetworkGlobals.h"

class TcpSender : public QObject
{
    Q_OBJECT

public:
    explicit TcpSender(QObject *parent = nullptr);
    ~TcpSender();

    void connectToHost(const QString &name, const QString &ipv4Address, int port);
    void sendDeviceInfo(int port);
    void sendMsg(const QString &msg);
    void sendBin(const QString &filePath);

signals:
    void canConnectSignal();

private:
    QTcpSocket *tcpSocketIPv4ForMsg = nullptr;
    QTcpSocket *tcpSocketIPv4ForBin = nullptr;
};

#endif