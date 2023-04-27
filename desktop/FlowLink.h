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
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QWidgetAction>

#include <memory>

#include "Globals.h"

#include "DockAreaWidget.h"
#include "DockManager.h"
#include "DockWidget.h"

#include "utils/TableModel.h"

#include "windows/ChatWindow.h"

#include "network/UdpSender.h"
#include "network/UdpReceiver.h"
#include "network/TcpSender.h"
#include "network/TcpReceiver.h"

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
  void setupLog();
  void setupDockManager();
  void createConnectionActionUi();
  void createChatActionUi();
  void createPerspectiveUi();
  void createCentralUI();
  void createDeviceTableUi();
  void createPropertiesTableUi();
  void addDevice(Device device);
  void removeDevices();

  Ui::FlowLink *ui;

  /* network */
  UdpReceiver *udpReceiver = nullptr;
  UdpSender *udpSender = nullptr;
  TcpReceiver *tcpReceiver = nullptr;
  TcpSender *tcpSender = nullptr;

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
  ChatWindow *chatWindow = nullptr;
  TableModel *deviceTableModel = nullptr;
  QSortFilterProxyModel *deviceProxyModel = nullptr;
  QTableView *deviceTableView = nullptr;
  QTableWidget *propertiesTable = nullptr;

private slots:
  void onConnectActionClicked();
  void onDisconnectActionClicked();
  void onChatActionClicked();
  void savePerspective();
};
#endif // FLOWLINK_H
