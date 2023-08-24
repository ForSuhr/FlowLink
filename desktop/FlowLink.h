#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <QTranslator>
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

#include "windows/PrefWindow.h"
#include "windows/ChatWindow.h"

#include "network/UdpSender.h"
#include "network/UdpReceiver.h"

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
  void setupMenuBar();
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
  int m_port = 8000;

  // dockmanager
  ads::CDockManager *m_dockManager = nullptr;
  ads::CDockAreaWidget *m_centralDockArea = nullptr;
  ads::CDockWidget *m_centralDockWidget = nullptr;

  /* menubar */
  PrefWindow *prefWindow = nullptr;

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
  QLabel *centralDockLabel = nullptr;
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
