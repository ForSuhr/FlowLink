#include <QCborValue>
#include <QCborMap>
#include <QVariantMap>

#include "TcpReceiver.h"

TcpReceiver::TcpReceiver(QObject *parent)
    : QObject(parent),
      server(new QTcpServer),
      tcpSocketIPv4(new QTcpSocket)
{
    createConnection();
}

TcpReceiver::~TcpReceiver()
{
}

void TcpReceiver::createConnection()
{
    // Listen for incoming connections on port 8000
    if (!server->listen(QHostAddress::AnyIPv4, 8000))
    {
        PLOG_DEBUG << "Failed to start TCP server";
        return;
    }
    else
    {
        PLOG_DEBUG << "Start TCP server successfully";
    }

    connect(server, &QTcpServer::newConnection, this, &TcpReceiver::handleNewConnection);
}

void TcpReceiver::handleNewConnection()
{
    tcpSocketIPv4 = server->nextPendingConnection();
    connect(tcpSocketIPv4, &QTcpSocket::readyRead, this, &TcpReceiver::processPendingDatagrams);
    PLOG_DEBUG << "TCP connection established";
}

void TcpReceiver::processPendingDatagrams()
{
    QByteArray datagram = tcpSocketIPv4->readAll();

    QCborStreamReader reader(datagram);
    QCborValue contents = QCborValue::fromCbor(reader);
    QCborMap cMap;
    QVariantMap vMap;

    switch (contents.type())
    {
    case QCborValue::Map:
    {
        cMap = contents.toMap();
        vMap = cMap.toVariantMap();
        break;
    }
    default:
    {
        qDebug() << "Error: top-level item is not a QCborMap";
        break;
    }
    }

    PLOG_DEBUG << vMap.value("0").toString();

    return;
}
