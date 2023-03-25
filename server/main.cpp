#include "FlowLink.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    FlowLink w;
    w.show();
    return a.exec();
}
