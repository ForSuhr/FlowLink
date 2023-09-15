#include "NetworkGlobals.h"

static int port = 8079;

Device localHostName()
{
    Device device;
    QHostInfo hostInfo;
    device.name = hostInfo.localHostName();
    device.port = port + 2;
    return device;
}

/**
 * @brief construct a header for the specified file
 * @param fileSize size of the file in bytes
 * */
QVariantMap header(ContentType contentType, const QString &filePath)
{
    int fileSize = 0;

    // Fill out the header, plaintext does not have fileName or fileSize
    QVariantMap *headerVMap = new QVariantMap();
    (*headerVMap)["contentType"] = contentType;
    if (contentType == ContentType::Binary) // check if the file is of type Binary
    {
        (*headerVMap)["fileName"] = QFileInfo(filePath).fileName();

        QFile f(filePath);
        if (f.open(QIODevice::ReadOnly))
        {
            (*headerVMap)["fileSize"] = f.readAll().size();
            f.close();
        }
    }

    return *headerVMap;
}
