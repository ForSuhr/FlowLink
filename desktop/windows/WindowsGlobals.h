#ifndef WINDOWS_GLOBALS_H
#define WINDOWS_GLOBALS_H

#include <QtEndian>
#include <QString>

#include "../Globals.h"

struct ConvertedNumber
{
    double number;
    QString unit;
};

inline ConvertedNumber BytesConvert(qint64 bytes)
{
    ConvertedNumber result;

    if (bytes < 1024)
    {
        result.number = static_cast<qreal>(bytes);
        result.unit = "B";
    }
    else if (bytes < 1024 * 1024)
    {
        result.number = static_cast<qreal>(bytes) / 1024;
        result.unit = "kB";
    }
    else if (bytes < 1024 * 1024 * 1024)
    {
        result.number = static_cast<qreal>(bytes) / (1024 * 1024);
        result.unit = "MB";
    }
    else
    {
        result.number = static_cast<qreal>(bytes) / (1024 * 1024 * 1024);
        result.unit = "GB";
    }

    return result;
}

#endif // WINDOWS_GLOBALS_H