#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <QMainWindow>
#include <QWidgetAction>
#include <QComboBox>

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

protected:
    virtual void closeEvent(QCloseEvent *event) override;

private:
    void createPerspectiveUi();

    Ui::FlowLink *ui;

    // dockwidget Perspective
    QAction *savePerspectiveAction = nullptr;
    QWidgetAction *perspectiveListAction = nullptr;
    QComboBox *perspectiveComboBox = nullptr;

    // dockmanager
    ads::CDockManager *dockManager;

private Q_SLOTS:
    void savePerspective();
};
#endif // FLOWLINK_H
