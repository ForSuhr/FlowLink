#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <QComboBox>
#include <QDebug>
#include <QInputDialog>
#include <QLabel>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QTableView>
#include <QTableWidget>
#include <QWidgetAction>
#include <memory>

#include "./utilities/TableModel.h"
#include "DockAreaWidget.h"
#include "DockManager.h"
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
  void setupDockManager();
  void createConnectionUi();
  void createPerspectiveUi();
  void createCentralUI();
  void createDeviceTableUi();
  void createPropertiesTableUi();
  void addDevice(Host host);

  Ui::FlowLink *ui;
  Receiver *receiver;

  // dockmanager
  ads::CDockManager *dockManager;
  ads::CDockAreaWidget *centralDockArea;

  // toolbar
  QAction *connectAction = nullptr;

  // perspective
  QAction *savePerspectiveAction = nullptr;
  QWidgetAction *perspectiveListAction = nullptr;
  QComboBox *perspectiveComboBox = nullptr;

  // dockwidget
  TableModel *deviceTableModel;
  QSortFilterProxyModel *deviceProxyModel;
  QTableView *deviceTableView;

private Q_SLOTS:
  void savePerspective();
};
#endif // FLOWLINK_H
