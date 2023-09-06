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
    QByteArray ba = tcpSocketIPv4->readAll();

    // get header size
    QByteArray baHeaderSize = ba.left(SIZE_OF_LONG_LONG);
    QDataStream dataStream1(&baHeaderSize, QIODevice::ReadOnly);
    long long int headerSize;
    dataStream1 >> headerSize;

    // get header
    QByteArray baHeader = ba.mid(SIZE_OF_LONG_LONG, headerSize);
    QDataStream dataStream2(&baHeader, QIODevice::ReadOnly);
    QVariantMap *headerVMap = new QVariantMap();
    dataStream2 >> *headerVMap;

    // get file size and file name from the header
    int contentType = (*headerVMap)["contentType"].toInt();
    qlonglong totalFileBytes = (*headerVMap)["totalBytes"].toLongLong();
    QString fileName = (*headerVMap)["fileName"].toString();

    // get body
    QByteArray baBody = ba.mid(SIZE_OF_LONG_LONG + headerSize);
    qlonglong receivedFileBytes = baBody.size();

    // use QCborStreamReader to read the bytearray body
    QCborStreamReader reader(baBody);

    if (contentType == ContentType::Binary)
    {
        PLOG_DEBUG << "totalFileBytes is " << totalFileBytes;
        PLOG_DEBUG << "receivedFileBytes is " << receivedFileBytes;

        while (receivedFileBytes < totalFileBytes)
        {
            tcpSocketIPv4->waitForReadyRead();

            QByteArray baCurrent = tcpSocketIPv4->readAll();

            reader.addData(baCurrent);
            receivedFileBytes += baCurrent.size();
            PLOG_DEBUG << "receivedFileBytes is " << receivedFileBytes;
        }
    }

    QCborValue contents = QCborValue::fromCbor(reader);
    QCborMap cMap;
    QVariantMap vMap;

    switch (contents.type())
    {
    case QCborValue::Map:
    {
        cMap = contents.toMap();
        vMap = cMap.toVariantMap();
        parserMap(vMap, fileName);
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

void TcpReceiver::parserMap(const QVariantMap &vMap, const QString &fileName = "new_file.bin")
{
    QString key;
    foreach (key, vMap.keys())
    {
        switch (key.toInt())
        {
        case ContentType::PlainText:
        {
            QString msg = vMap.value(key).toString();
            emit msgSignal(msg);
            break;
        }
        case ContentType::Binary:
        {
            PLOG_DEBUG << "Info: received a binary data.";
            QFile f(config.value("common/downloadDirectory").toString() + "/" + fileName);
            if (f.open(QIODevice::WriteOnly))
            {
                QByteArray ba = vMap.value(key).toByteArray();
                f.write(ba);
                f.close();
            }
            break;
        }
        default:
        {
            PLOG_DEBUG << "Error: no valid datatype in QCborMap.";
            break;
        }
        }
    }
}
