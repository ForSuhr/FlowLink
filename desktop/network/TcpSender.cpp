#include "TcpSender.h"

TcpSender::TcpSender(const QString &ipv4Address, QObject *parent)
    : QObject(parent),
      tcpSocketIPv4(new QTcpSocket)
{
    tcpSocketIPv4->connectToHost(ipv4Address, 8000);

    if (!tcpSocketIPv4->waitForConnected(5000))
    {
        PLOG_DEBUG << "Failed to connect to host";
    }
    else
    {
        PLOG_DEBUG << "Connected to host successfully";
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

    tcpSocketIPv4->write(baHeaderSize + baHeader + baBody);
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

    tcpSocketIPv4->write(baHeaderSize + baHeader + baBody);
    tcpSocketIPv4->waitForBytesWritten(-1);
}
