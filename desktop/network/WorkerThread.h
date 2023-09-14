#ifndef WORKER_POOL
#define WORKER_POOL

#include <QThread>
#include <QObject>
#include <QCborValue>
#include <QCborMap>

#include "NetworkGlobals.h"

class WorkerThread : public QObject
{

    Q_OBJECT

public:
    WorkerThread(QObject *parent = nullptr);
    ~WorkerThread();

    void parserMap(const QVariantMap &vMap, const QString &fileName);

public slots:
    void createConnection();
    void handleNewConnection();
    void processPendingDatagrams();

signals:
    void msgSignal(const QString &msg);
    void startNewTaskSignal(const QString &filename, qint64 totalFileBytes);
    void updateProgressSignal(const QString &filename, qint64 receivedBytes, qint64 totalBytes);

private:
    QTcpServer *m_server = nullptr;
    QTcpSocket *m_tcpSocketIPv4 = nullptr;
};

#endif // WORKER_POOL