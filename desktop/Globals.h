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

/* Translate File Path */
void setUpLanguageMap(QMap<QString, QString> &map);

#endif // GLOBALS_H