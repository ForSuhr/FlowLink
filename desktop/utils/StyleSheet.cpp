#include "StyleSheet.h"

StyleSheet &StyleSheet::Instance()
{
    static StyleSheet instance;
    return instance;
}

void StyleSheet::loadQSS(QWidget *widget, QString qssPath)
{
    QFile file(qssPath);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QString qss = file.readAll();
        widget->setStyleSheet(qss);
    }

    file.close();
}

StyleSheet::StyleSheet()
{
}

StyleSheet::~StyleSheet()
{
}
