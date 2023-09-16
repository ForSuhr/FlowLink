#include "TcpReceiver.h"

TcpReceiver::TcpReceiver(QObject *parent)
    : QObject(parent)
{
    // add a new worker for handling messages
    m_workerForMsg = new WorkerThread();
    connect(this, &TcpReceiver::threadForMsgStartedSignal, m_workerForMsg, &WorkerThread::listenToPort);
    connect(this, &TcpReceiver::connectionForMsgClosedSignal, m_workerForMsg, &WorkerThread::closeConnection);
    connect(m_workerForMsg, &WorkerThread::msgSignal, this, &TcpReceiver::msgSignal);
    connect(m_workerForMsg, &WorkerThread::receivedDeviceInfoViaTcp, this, &TcpReceiver::receivedDeviceInfoViaTcp);

    // then do the same for handling binary files
    m_workerForBin = new WorkerThread();
    connect(this, &TcpReceiver::threadForBinStartedSignal, m_workerForBin, &WorkerThread::listenToPort);
    connect(this, &TcpReceiver::connectionForBinClosedSignal, m_workerForBin, &WorkerThread::closeConnection);
    connect(m_workerForBin, &WorkerThread::startNewTaskSignal, this, &TcpReceiver::startNewTaskSignal);
    connect(m_workerForBin, &WorkerThread::updateProgressSignal, this, &TcpReceiver::updateProgressSignal);

    // create new thread to work on establishing new connection and handling pending datagrams
    m_threadForMsg = new QThread(this);
    m_workerForMsg->moveToThread(m_threadForMsg);
    m_threadForMsg->start();

    m_threadForBin = new QThread(this);
    m_workerForBin->moveToThread(m_threadForMsg);
    m_threadForBin->start();
}

TcpReceiver::~TcpReceiver()
{
}

void TcpReceiver::listenToPort(int port)
{
    emit threadForMsgStartedSignal(port);
    emit threadForBinStartedSignal(port + 1);
}

void TcpReceiver::closeConnection()
{
    emit connectionForMsgClosedSignal();
    emit connectionForBinClosedSignal();
}
