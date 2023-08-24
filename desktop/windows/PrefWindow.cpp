#include "PrefWindow.h"

PrefWindow::PrefWindow(QWidget *parent) : TabWidget(parent)
{
    addTab(new QWidget, tr("Appearance"));

    setWindowTitle(tr("Preferences"));
    resize(500, 600);
}
