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
    void disconnectFromHost();
    void sendDeviceInfo(int port);
    void sendMsg(const QString &msg);
    void sendBin(const QString &filePath);

signals:
    void canConnectSignal();

private:
    QTcpSocket *m_tcpSocketIPv4ForMsg = nullptr;
    QTcpSocket *m_tcpSocketIPv4ForBin = nullptr;
};

#endif