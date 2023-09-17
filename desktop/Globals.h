#ifndef GLOBALS_H
#define GLOBALS_H

#include <Log.h>
#include <Init.h>
#include <Formatters/TxtFormatter.h>
#include <Appenders/ConsoleAppender.h>
#include <Appenders/ColorConsoleAppender.h>
#include <Appenders/RollingFileAppender.h>

#include <QMap>
#include <QString>

#include "utils/ConfigParser.h"

extern QString licensesText;

// translate files path
void setUpLanguageMap(QMap<QString, QString> &map);

// stylesheet files path
void setUpStylesheetMap(QMap<QString, QString> &map);

#endif // GLOBALS_H