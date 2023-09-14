#ifndef TCP_RECEIVER_H
#define TCP_RECEIVER_H

#include "NetworkGlobals.h"
#include "WorkerThread.h"

class TcpReceiver : public QObject
{
    Q_OBJECT

public:
    explicit TcpReceiver(QObject *parent = nullptr);
    ~TcpReceiver();

signals:
    void threadForMsgStartedSignal(int port);
    void threadForBinStartedSignal(int port);
    void msgSignal(const QString &msg);
    void startNewTaskSignal(const QString &filename, qint64 totalFileBytes);
    void updateProgressSignal(const QString &filename, qint64 receivedBytes, qint64 totalBytes);
};

#endif // TCP_RECEIVER_H