#include "FlowLink.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    /* load translate file */
    QTranslator translator;
    QMap<QString, QString> languageMap;
    setUpLanguageMap(languageMap);
    const QString baseName = config.value("Appearance/Language").toString();
    if (translator.load(languageMap[baseName]))
    {
        a.installTranslator(&translator);
    }

    FlowLink w;
    w.show();
    return a.exec();
}
