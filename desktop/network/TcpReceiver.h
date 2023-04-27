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

private:
    void handleStream();

    QTcpServer *server = nullptr;
};

#endif