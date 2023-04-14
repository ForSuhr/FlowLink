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
#include <QItemSelectionModel>
#include <QWidgetAction>
#include <memory>

#include "./utilities/TableModel.h"
#include "DockAreaWidget.h"
#include "DockManager.h"
#include "DockWidget.h"
#include "Chat.h"
#include "Receiver.h"
#include "Sender.h"

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
  void createChatUi();
  void createPerspectiveUi();
  void createCentralUI();
  void createDeviceTableUi();
  void createPropertiesTableUi();
  void addDevice(Device device);
  void removeDevices();

  Ui::FlowLink *ui;
  Receiver *receiver = nullptr;
  Sender *sender = nullptr;

  // dockmanager
  ads::CDockManager *dockManager = nullptr;
  ads::CDockAreaWidget *centralDockArea = nullptr;

  // toolbar
  QAction *connectAction = nullptr;
  QAction *disconnectAction = nullptr;
  QAction *chatAction = nullptr;

  // perspective
  QAction *savePerspectiveAction = nullptr;
  QWidgetAction *perspectiveListAction = nullptr;
  QComboBox *perspectiveComboBox = nullptr;

  // dockwidget
  TableModel *deviceTableModel = nullptr;
  QSortFilterProxyModel *deviceProxyModel = nullptr;
  QTableView *deviceTableView = nullptr;

private Q_SLOTS:
  void onConnectActionClicked();
  void onDisconnectActionClicked();
  void onChatActionClicked();
  void savePerspective();
};
#endif // FLOWLINK_H
