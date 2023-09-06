#ifndef NETWORK_GLOBALS_H
#define NETWORK_GLOBALS_H

#include <QFile>
#include <QFileInfo>
#include <QMap>
#include <QHostAddress>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTcpServer>
#include <QHostInfo>
#include <QDataStream>
#include <QNetworkInterface>
#include <QCborStreamWriter>
#include <QCborStreamReader>

#include <Log.h>
#include <Init.h>
#include <Formatters/TxtFormatter.h>
#include <Appenders/ConsoleAppender.h>
#include <Appenders/ColorConsoleAppender.h>
#include <Appenders/RollingFileAppender.h>

#include "../utils/ConfigParser.h"

#define SIZE_OF_LONG_LONG 8

struct Device
{
    QString name = "NA";
    QString address = "NA";

    bool operator==(const Device &other) const
    {
        return name == other.name && address == other.address;
    }
};

enum ContentType
{
    PlainText,
    Binary,
};

inline QDataStream &operator<<(QDataStream &stream, const Device &device)
{
    return stream << device.name << device.address;
}

inline QDataStream &operator>>(QDataStream &stream, Device &device)
{
    return stream >> device.name >> device.address;
}

/**
 * @brief get device name and address
 * @return struct Device
 */
Device localHostName();

QVariantMap header(ContentType contentType, const QString &filePath = "");

#endif