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
#include <unordered_map>

#include "Globals.h"

#include "DockAreaWidget.h"
#include "DockManager.h"
#include "DockWidget.h"

#include "utils/ConfigParser.h"
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
  void loadPreferences();

  void addDevice(Device device);
  void removeDevices();

  Ui::FlowLink *ui;

  /* network */
  UdpReceiver *m_udpReceiver = nullptr;
  UdpSender *m_udpSender = nullptr;
  TcpReceiver *m_tcpReceiver = nullptr;
  TcpSender *m_tcpSender = nullptr;

  // dockmanager
  ads::CDockManager *m_dockManager = nullptr;
  ads::CDockAreaWidget *m_centralDockArea = nullptr;
  ads::CDockWidget *m_centralDockWidget = nullptr;

  // toolbar
  QAction *m_connectAction = nullptr;
  QAction *m_disconnectAction = nullptr;
  QAction *m_chatAction = nullptr;

  // perspective
  QAction *m_savePerspectiveAction = nullptr;
  QWidgetAction *m_perspectiveListAction = nullptr;
  QAction *m_setDefaultPerspective = nullptr;
  QComboBox *m_perspectiveComboBox = nullptr;

  // dockwidget
  TableModel *m_deviceTableModel = nullptr;
  QSortFilterProxyModel *m_deviceProxyModel = nullptr;
  QTableView *m_deviceTableView = nullptr;
  QTableWidget *m_propertiesTable = nullptr;

  // field for dockwidget
  std::unordered_map<QString, ChatWindow *> m_chatWindowMap = {}; // a map used to store chat window pointers

private slots:
  void onConnectActionClicked();
  void onDisconnectActionClicked();
  void onChatActionClicked();
  void savePerspective();
};
#endif // FLOWLINK_H
