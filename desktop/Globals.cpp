#include "Globals.h"

void setUpLanguageMap(QMap<QString, QString> &map)
{
    map["English"] = QString("./translate/en_US.qm");
    map["中文"] = QString("./translate/zh_CN.qm");
    map["Deutsch"] = QString("./translate/de_DE.qm");
}

void setUpStylesheetMap(QMap<QString, QString> &map)
{
    map["Lumos"] = QString(":/asset/style/lumos/lumos.qss");
}
