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

    QByteArray datagram;
    QCborStreamWriter writer(&datagram);

    writer.startMap();
    writer.append(DataType::PlainText);
    writer.append(msg);
    writer.endMap();

    tcpSocketIPv4->write(datagram);
}
