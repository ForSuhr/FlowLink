#include "TcpSender.h"

TcpSender::TcpSender(QObject *parent)
    : QObject(parent),
      tcpSocketIPv4ForMsg(new QTcpSocket),
      tcpSocketIPv4ForBin(new QTcpSocket)
{
}

TcpSender::~TcpSender()
{
}

void TcpSender::connectToHost(const QString &name, const QString &ipv4Address, int port)
{
    tcpSocketIPv4ForMsg->connectToHost(ipv4Address, port);
    bool canConnectMsg = tcpSocketIPv4ForMsg->waitForConnected(5000);

    if (!canConnectMsg)
    {
        PLOG_DEBUG << "Msg socket: Failed to connect to host " << ipv4Address << " " << port;
    }
    else
    {
        PLOG_DEBUG << "Msg socket: Connected to host successfully " << ipv4Address << " " << port;
    }

    tcpSocketIPv4ForBin->connectToHost(ipv4Address, port + 1);
    bool canConnectBin = tcpSocketIPv4ForBin->waitForConnected(5000);

    if (!canConnectBin)
    {
        PLOG_DEBUG << "Bin socket: Failed to connect to host " << ipv4Address << " " << port + 1;
    }
    else
    {
        PLOG_DEBUG << "Bin socket: Connected to host successfully " << ipv4Address << " " << port + 1;
    }

    if (canConnectMsg & canConnectBin)
        emit canConnectSignal(); // client: send back signal to create a chat window

    // client: send tcp signal to notify the server of the client device info
    sendDeviceInfo(port);
}

void TcpSender::sendDeviceInfo(int port)
{
    Device device = localHostName();
    device.address = "NA";
    device.port = port;
    QByteArray baDeviceInfo;
    QDataStream stream1(&baDeviceInfo, QIODevice::ReadWrite);
    stream1 << device;

    QByteArray baBody;
    QCborStreamWriter writer(&baBody);

    writer.startMap();
    writer.append(ContentType::DeviceInfo);
    writer.append(baDeviceInfo);
    writer.endMap();

    // convert the file header to QByteArray
    QByteArray baHeader;
    QDataStream stream2(&baHeader, QIODevice::WriteOnly);
    QVariantMap headerVMap = header(ContentType::DeviceInfo);
    stream2 << headerVMap;
    qlonglong headerSize = baHeader.size();

    // convert the header size to QByteArray
    QByteArray baHeaderSize;
    QDataStream stream3(&baHeaderSize, QIODevice::WriteOnly);
    stream3 << headerSize;

    tcpSocketIPv4ForMsg->write(baHeaderSize + baHeader + baBody);
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
