#include "TcpReceiver.h"

TcpReceiver::TcpReceiver(QObject *parent)
    : QObject(parent)
{
    // add a new worker for handling messages
    workerForMsg = new WorkerThread();
    connect(this, &TcpReceiver::threadForMsgStartedSignal, workerForMsg, &WorkerThread::createConnection);
    connect(this, &TcpReceiver::connectionForMsgClosedSignal, workerForMsg, &WorkerThread::closeConnection);
    connect(workerForMsg, &WorkerThread::msgSignal, this, &TcpReceiver::msgSignal);

    // leave the new thread to work on establishing new connection and handling pending datagrams
    QThread *threadForMsg = new QThread(this);
    workerForMsg->moveToThread(threadForMsg);
    threadForMsg->start();
    emit threadForMsgStartedSignal(8080);

    // then do the same for handling binary files
    workerForBin = new WorkerThread();
    connect(this, &TcpReceiver::threadForBinStartedSignal, workerForBin, &WorkerThread::createConnection);
    connect(this, &TcpReceiver::connectionForBinClosedSignal, workerForBin, &WorkerThread::closeConnection);
    connect(workerForBin, &WorkerThread::startNewTaskSignal, this, &TcpReceiver::startNewTaskSignal);
    connect(workerForBin, &WorkerThread::updateProgressSignal, this, &TcpReceiver::updateProgressSignal);

    QThread *threadForBin = new QThread(this);
    workerForBin->moveToThread(threadForMsg);
    threadForBin->start();
    emit threadForBinStartedSignal(8081);
}

TcpReceiver::~TcpReceiver()
{
}

void TcpReceiver::closeConnection()
{
    emit connectionForMsgClosedSignal();
    emit connectionForBinClosedSignal();
}
