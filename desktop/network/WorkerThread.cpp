#include "WorkerThread.h"

WorkerThread::WorkerThread(QObject *parent) : QObject(parent)
{
}

WorkerThread::~WorkerThread()
{
}

void WorkerThread::listenToPort(int port)
{
    m_server = new QTcpServer();

    // Listen for incoming connections on port
    if (!m_server->listen(QHostAddress::AnyIPv4, port))
    {
        PLOG_DEBUG << "Failed to start TCP server";
        return;
    }
    else
    {
        PLOG_DEBUG << "Start TCP server successfully, listening on port: " << port;
    }

    connect(m_server, &QTcpServer::newConnection, this, &WorkerThread::handleNewConnection);
}

void WorkerThread::closeConnection()
{
    if (m_tcpSocketIPv4 != nullptr)
    {
        if (m_tcpSocketIPv4->ConnectedState == QAbstractSocket::ConnectedState)
            m_tcpSocketIPv4->close();
    }
    if (m_server->isListening())
        m_server->close();
    PLOG_DEBUG << "All connections closed";
}

void WorkerThread::handleNewConnection()
{
    m_tcpSocketIPv4 = m_server->nextPendingConnection();
    connect(m_tcpSocketIPv4, &QTcpSocket::readyRead, this, &WorkerThread::processPendingDatagrams);
}

void WorkerThread::processPendingDatagrams()
{
    PLOG_DEBUG << "running on: " << QThread::currentThread();

    QByteArray ba = m_tcpSocketIPv4->readAll();

    // get header size
    QByteArray baHeaderSize = ba.left(SIZE_OF_LONG_LONG);
    QDataStream dataStream1(&baHeaderSize, QIODevice::ReadOnly);
    long long int headerSize;
    dataStream1 >> headerSize;

    // get header
    QByteArray baHeader = ba.mid(SIZE_OF_LONG_LONG, headerSize);
    QDataStream dataStream2(&baHeader, QIODevice::ReadOnly);
    QVariantMap *headerVMap = new QVariantMap();
    dataStream2 >> *headerVMap;

    // get file size and file name from the header
    int contentType = (*headerVMap)["contentType"].toInt();
    qlonglong totalFileBytes = (*headerVMap)["totalBytes"].toLongLong();
    QString fileName = (*headerVMap)["fileName"].toString();

    // get body
    QByteArray baBody = ba.mid(SIZE_OF_LONG_LONG + headerSize);
    qlonglong receivedFileBytes = baBody.size();

    // use QCborStreamReader to read the bytearray body
    QCborStreamReader reader(baBody);

    if (contentType == ContentType::Binary)
    {
        PLOG_DEBUG << "totalFileBytes is " << totalFileBytes;
        PLOG_DEBUG << "receivedFileBytes is " << receivedFileBytes;

        // emit a signal to notify the frontend that a new download task has started
        emit startNewTaskSignal(fileName, totalFileBytes);

        while (receivedFileBytes < totalFileBytes)
        {
            // update progress
            emit updateProgressSignal(fileName, receivedFileBytes, totalFileBytes);

            // wait and read all available bytes from the socket
            m_tcpSocketIPv4->waitForReadyRead();
            QByteArray baCurrent = m_tcpSocketIPv4->readAll();

            // add new data, update receivedFileBytes
            reader.addData(baCurrent);
            receivedFileBytes += baCurrent.size();
            PLOG_DEBUG << "receivedFileBytes is " << receivedFileBytes;
        }

        emit updateProgressSignal(fileName, receivedFileBytes, totalFileBytes);
    }

    QCborValue contents = QCborValue::fromCbor(reader);
    QCborMap cMap;
    QVariantMap vMap;

    switch (contents.type())
    {
    case QCborValue::Map:
    {
        cMap = contents.toMap();
        vMap = cMap.toVariantMap();
        parserMap(vMap, fileName);
        break;
    }
    default:
    {
        PLOG_DEBUG << "Error: top-level item is not a QCborMap";
        break;
    }
    }
}

void WorkerThread::parserMap(const QVariantMap &vMap, const QString &fileName = "new_file.bin")
{
    QString key;
    foreach (key, vMap.keys())
    {
        switch (key.toInt())
        {
        case ContentType::PlainText:
        {
            QString msg = vMap.value(key).toString();
            emit msgSignal(msg);
            break;
        }
        case ContentType::Binary:
        {
            PLOG_DEBUG << "Info: received a binary data.";
            QFile f(config.value("common/downloadDirectory").toString() + "/" + fileName);
            if (f.open(QIODevice::WriteOnly))
            {
                QByteArray ba = vMap.value(key).toByteArray();
                f.write(ba);
                f.close();
            }
            break;
        }
        case ContentType::DeviceInfo:
        {
            PLOG_DEBUG << "Info: received device info.";
            // send a signal back, indicating that there is a new connection being established by your peer and now you received the device info of your peer via tcp socket
            QByteArray baDeviceInfo = vMap.value(key).toByteArray();
            QDataStream stream(&baDeviceInfo, QIODevice::ReadOnly);
            Device device;
            stream >> device;
            device.address = m_tcpSocketIPv4->peerAddress().toString();
            emit receivedDeviceInfoViaTcp(device.name, device.address, device.port);
            PLOG_DEBUG << "name: " << device.name << " addr: " << device.address << " port: " << device.port;
            break;
        }
        default:
        {
            PLOG_DEBUG << "Error: no valid datatype in QCborMap.";
            break;
        }
        }
    }
}
