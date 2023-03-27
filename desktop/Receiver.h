#ifndef RECEIVER_H
#define RECEIVER_H

#include <QHostAddress>
#include <QUdpSocket>

class Receiver: public QObject
{
    Q_OBJECT

public:
    explicit Receiver(QObject* parent = nullptr);
    ~Receiver();
    void createConnection();
    void closeConnection();

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket udpSocketIPv4;
    QHostAddress groupAddressIPv4;
};

#endif // RECEIVER_H
