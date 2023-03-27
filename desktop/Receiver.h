#ifndef RECEIVER_H
#define RECEIVER_H

#include <QHostAddress>
#include <QUdpSocket>
#include <QHostInfo>

struct Host
{
    QString name;
    QString address;
};

class Receiver : public QObject
{
    Q_OBJECT

public:
    explicit Receiver(QObject *parent = nullptr);
    ~Receiver();
    void createConnection();
    void closeConnection();

signals:
    void sendHostInfo(Host host);

private slots:
    void processPendingDatagrams();

private:
    Host host;
    QHostInfo hostInfo;
    QUdpSocket udpSocketIPv4;
    QHostAddress groupAddressIPv4;
};

#endif // RECEIVER_H
