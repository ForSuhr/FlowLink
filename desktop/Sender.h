#ifndef SENDER_H
#define SENDER_H

#include "utilities/globals.h"

class Sender : public QObject
{
    Q_OBJECT

public:
    explicit Sender(QObject *parent = nullptr);
    void sendDatagram();

private:
    Device device;
    QHostAddress groupAddressIPv4;
};

#endif