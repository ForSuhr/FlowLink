#include "TcpSender.h"

TcpSender::TcpSender(const QString &ipv4Address, QObject *parent)
    : QObject(parent),
      tcpSocketIPv4ForMsg(new QTcpSocket),
      tcpSocketIPv4ForBin(new QTcpSocket)
{
    tcpSocketIPv4ForMsg->connectToHost(ipv4Address, 8080);

    if (!tcpSocketIPv4ForMsg->waitForConnected(5000))
    {
        PLOG_DEBUG << "Msg socket: Failed to connect to host";
    }
    else
    {
        PLOG_DEBUG << "Msg socket: Connected to host successfully";
    }

    tcpSocketIPv4ForBin->connectToHost(ipv4Address, 8081);

    if (!tcpSocketIPv4ForBin->waitForConnected(5000))
    {
        PLOG_DEBUG << "Bin socket: Failed to connect to host";
    }
    else
    {
        PLOG_DEBUG << "Bin socket: Connected to host successfully";
    }
}

TcpSender::~TcpSender()
{
}

void TcpSender::sendMsg(const QString &msg)
{
    QByteArray baBody;
    QCborStreamWriter writer(&baBody);

    writer.startMap();
    writer.append(ContentType::PlainText);
    writer.append(msg);
    writer.endMap();

    // convert the file header to QByteArray
    QByteArray baHeader;
    QDataStream dataStream1(&baHeader, QIODevice::WriteOnly);
    QVariantMap headerVMap = header(ContentType::PlainText);
    dataStream1 << headerVMap;
    qlonglong headerSize = baHeader.size();

    // convert the header size to QByteArray
    QByteArray baHeaderSize;
    QDataStream dataStream2(&baHeaderSize, QIODevice::WriteOnly);
    dataStream2 << headerSize;

    tcpSocketIPv4ForMsg->write(baHeaderSize + baHeader + baBody);
}

void TcpSender::sendBin(const QString &filePath)
{
    QByteArray baBody;
    QCborStreamWriter writer(&baBody);

    writer.startMap();
    writer.append(ContentType::Binary);
    QFile f(filePath);
    if (f.open(QIODevice::ReadOnly))
    {
        writer.append(f.readAll());
        f.close();
    }
    writer.endMap();

    // convert the file header to QByteArray
    QByteArray baHeader;
    QDataStream dataStream1(&baHeader, QIODevice::WriteOnly);
    QVariantMap headerVMap = header(ContentType::Binary, filePath);
    headerVMap["totalBytes"] = baBody.size();
    dataStream1 << headerVMap;
    qlonglong headerSize = baHeader.size();

    // convert the header size to QByteArray
    QByteArray baHeaderSize;
    QDataStream dataStream2(&baHeaderSize, QIODevice::WriteOnly);
    dataStream2 << headerSize;

    tcpSocketIPv4ForBin->write(baHeaderSize + baHeader + baBody);
    tcpSocketIPv4ForBin->waitForBytesWritten(-1);
}
