#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H

#include "NetworkGlobals.h"

class TcpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit TcpReceiver(QObject *parent = nullptr);
    ~TcpReceiver();

    void createConnection();

signals:
    void msgSignal(const QString &msg);

private:
    void handleNewConnection();
    void processPendingDatagrams();
    void parserMap(const QVariantMap &vMap);

    QTcpServer *server = nullptr;
    QTcpSocket *tcpSocketIPv4 = nullptr;
};

#endif