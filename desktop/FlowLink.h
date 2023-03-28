#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <memory>
#include <list>
#include <QMainWindow>
#include <QWidgetAction>
#include <QComboBox>
#include <QTableWidget>
#include <QStringListModel>

#include "DockManager.h"
#include "DockAreaWidget.h"
#include "DockWidget.h"
#include "Receiver.h"

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

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void createConnectionUi();
    void createPerspectiveUi();
    void addDevice(Host host);

    Ui::FlowLink *ui;
    Receiver *receiver;

    // dockmanager
    ads::CDockManager *dockManager;

    // toolbar
    QAction *connectAction = nullptr;

    // perspective
    QAction *savePerspectiveAction = nullptr;
    QWidgetAction *perspectiveListAction = nullptr;
    QComboBox *perspectiveComboBox = nullptr;

    // dockwidget
    QTableWidget *deviceTable = nullptr;

    // fields
    std::list<QString> deviceList = {};

private Q_SLOTS:
    void savePerspective();
};
#endif // FLOWLINK_H
