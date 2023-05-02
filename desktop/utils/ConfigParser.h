/**
 * @example
 * ConfigParser::instance().beginGroup("Scenario");
 * ConfigParser::instance().setValue("Name", QString("test 1"));
 * ConfigParser::instance().setValue("Date/Year", QString("2023"));
 * ConfigParser::instance().setValue("Date/Month", QString("April"));
 * ConfigParser::instance().endGroup();
 */

#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "QSettingsJSON.h"

#define CONFIG_PATH "./config/settings.json"

/* config marco*/
#ifndef config
#define config ConfigParser::instance()
#endif

class ConfigParser : public QSettings
{
public:
    static ConfigParser &instance();

private:
    ConfigParser();
};

#endif // CONFIG_PARSER_H