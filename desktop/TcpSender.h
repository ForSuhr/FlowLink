#ifndef TCPSENDER_H
#define TCPSENDER_H

#include "globals.h"

class TcpSender : public QObject
{
    Q_OBJECT

public:
    explicit TcpSender(QObject *parent = nullptr);
    ~TcpSender();
};

#endif