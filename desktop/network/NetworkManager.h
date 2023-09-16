#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H

#include "../network/TcpSender.h"
#include "../network/TcpReceiver.h"

class NetworkManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkManager(QObject *parent = nullptr);
    ~NetworkManager();

    void listenToPort(int port);
    void connectToHost(QString name, QString address, int port);

    /* network */
    TcpReceiver *m_tcpReceiver = nullptr;
    TcpSender *m_tcpSender = nullptr;
    Device m_device;
};

#endif // NETWORK_MANAGER_H
