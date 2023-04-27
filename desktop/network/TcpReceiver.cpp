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
        qDebug() << "Error: failed to start server";
        return;
    }

    connect(server, &QTcpServer::newConnection, this, &TcpReceiver::handleNewConnection);
    connect(tcpSocketIPv4, &QTcpSocket::readyRead, this, &TcpReceiver::processPendingDatagrams);
}

void TcpReceiver::handleNewConnection()
{
    tcpSocketIPv4 = server->nextPendingConnection();
}

void TcpReceiver::processPendingDatagrams()
{
    QCborStreamReader reader(tcpSocketIPv4);
    QCborValue contents = QCborValue::fromCbor(reader);
    QCborMap cMap;
    QVariantMap vMap;

    // Check that the top-level item (cbor stream from tcpSocketIPv4) is a map
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
        qDebug() << "Error: top-level item is not a map";
        break;
    }
    }

    tcpSocketIPv4->close();
    delete tcpSocketIPv4;

    QString tmp = vMap.value("0").toString();
    qDebug().noquote() << tmp;

    return;
}
