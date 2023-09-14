#include "TcpReceiver.h"

TcpReceiver::TcpReceiver(QObject *parent)
    : QObject(parent)
{
    // add a new worker
    WorkerThread *worker = new WorkerThread();
    connect(this, &TcpReceiver::threadStartedSignal, worker, &WorkerThread::createConnection);
    connect(worker, &WorkerThread::msgSignal, this, &TcpReceiver::msgSignal);
    connect(worker, &WorkerThread::startNewTaskSignal, this, &TcpReceiver::startNewTaskSignal);
    connect(worker, &WorkerThread::updateProgressSignal, this, &TcpReceiver::updateProgressSignal);

    // leave the new thread to work on establishing new connection and handling pending datagrams
    QThread *thread = new QThread(this);
    worker->moveToThread(thread);
    thread->start();
    emit threadStartedSignal();
}

TcpReceiver::~TcpReceiver()
{
}
