#include "NetworkManager.h"

NetworkManager::NetworkManager(QObject *parent)
{
    m_tcpReceiver = new TcpReceiver();
    m_tcpSender = new TcpSender();
}

NetworkManager::~NetworkManager()
{
}

/// @brief initialize tcp receiver, i.e. set up a tcp server to listen to the given port
/// @param port
void NetworkManager::listenToPort(int port)
{
    m_tcpReceiver->listenToPort(port);
}

/// @brief connect to the device with the given address and port, i.e. establish a tcp socket connection
/// @param address
/// @param port
void NetworkManager::connectToHost(QString name, QString address, int port)
{
    m_device.name = name;
    m_device.address = address;
    m_device.port = port;
    m_tcpSender->connectToHost(name, address, port);
}
