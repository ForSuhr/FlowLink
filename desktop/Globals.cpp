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

QString licensesText = "Qt - www.qt.io\nLicense : GNU LGPL version 3\n\nQt ADS - https : // github.com/githubuser0xFFFF/Qt-Advanced-Docking-System\nLicense : LGPL v2 .1\n\nplog - https : // github.com/SergiusTheBest/plog\nLicense: MIT License ";
