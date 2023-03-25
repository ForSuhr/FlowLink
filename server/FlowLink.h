#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <QMainWindow>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class FlowLink;
}
QT_END_NAMESPACE

class FlowLink : public QMainWindow
{
    Q_OBJECT

public:
    FlowLink(QWidget *parent = nullptr);
    ~FlowLink();

private:
    Ui::FlowLink *ui;

    ads::CDockManager *dockManager;
    ads::CDockAreaWidget *statusDockArea;
    ads::CDockWidget *timelineDockWidget;
};
#endif // FLOWLINK_H
