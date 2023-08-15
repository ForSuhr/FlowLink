#include <QCborValue>
#include <QCborMap>
#include <QVariantMap>

#include "TcpReceiver.h"

TcpReceiver::TcpReceiver(int port, QObject *parent)
    : QObject(parent),
      server(new QTcpServer),
      tcpSocketIPv4(new QTcpSocket)
{
    createConnection(port);
}

TcpReceiver::~TcpReceiver()
{
}

void TcpReceiver::createConnection(int port)
{
    // Listen for incoming connections on port
    if (!server->listen(QHostAddress::AnyIPv4, port))
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
        parserMap(vMap);
        break;
    }
    default:
    {
        PLOG_DEBUG << "Error: top-level item is not a QCborMap";
        break;
    }
    }

    return;
}

void TcpReceiver::parserMap(const QVariantMap &vMap)
{
    QString key;
    foreach (key, vMap.keys())
    {
        switch (key.toInt())
        {
        case DataType::PlainText:
        {
            emit msgSignal(vMap.value("0").toString());
            break;
        }
        default:
        {
            PLOG_DEBUG << "Error: no valid datatype in QCborMap";
            break;
        }
        }
    }
}
