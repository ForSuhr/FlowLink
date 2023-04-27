#include "TcpSender.h"

TcpSender::TcpSender(const QString &ipv4Address, QObject *parent)
    : QObject(parent),
      tcpSocketIPv4(new QTcpSocket)
{
    tcpSocketIPv4->connectToHost(ipv4Address, 8000);

    if (!tcpSocketIPv4->waitForConnected(5000))
    {
        qDebug() << "Error: failed to connect to host";
    }
}

TcpSender::~TcpSender()
{
}

void TcpSender::sendMsg(const QString &msg)
{
    QCborStreamWriter writer(tcpSocketIPv4);

    writer.startMap();
    writer.append(DataType::PlainText);
    writer.append(msg);
    writer.endMap();
}
