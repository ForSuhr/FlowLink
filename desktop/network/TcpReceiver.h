#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H

#include "NetworkGlobals.h"

class TcpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit TcpReceiver(int port, QObject *parent = nullptr);
    ~TcpReceiver();

signals:
    void msgSignal(const QString &msg);

private:
    void createConnection(int port = 8000);
    void handleNewConnection();
    void processPendingDatagrams();
    void parserMap(const QVariantMap &vMap);

    QTcpServer *server = nullptr;
    QTcpSocket *tcpSocketIPv4 = nullptr;
};

#endif