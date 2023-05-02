#include "ConfigParser.h"

ConfigParser::ConfigParser() : QSettings(CONFIG_PATH, JsonFormat)
{
}

/**
 * @brief Get the singleton instance
 * @return the singleton instance
 */
ConfigParser &ConfigParser::instance()
{
    static ConfigParser instance;
    return instance;
}