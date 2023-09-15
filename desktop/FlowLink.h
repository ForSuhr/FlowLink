#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <QTranslator>
#include <QComboBox>
#include <QDebug>
#include <QInputDialog>
#include <QLabel>
#include <QIcon>
#include <QMainWindow>
#include <QSortFilterProxyModel>
#include <QStringListModel>
#include <QTableView>
#include <QTableWidget>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QWidgetAction>
#include <QToolButton>
#include <QMessageBox>
#include <QTimer>

#include <memory>
#include <unordered_map>

#include "Globals.h"

#include "DockAreaWidget.h"
#include "DockManager.h"
#include "DockWidget.h"

#include "utils/ConfigParser.h"
#include "utils/StyleSheet.h"
#include "utils/TableModel.h"

#include "windows/PrefWindow.h"
#include "windows/ChatWindow.h"
#include "windows/ProgressWindow.h"

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

private slots:
  void onConnectActionClicked();
  void onDisconnectActionClicked();
  void onToggleShowLocalHostActionClicked();
  void openChatWindow();
  void NewPerspective();
  void SaveAsCurrentPerspective();
  void deletePerspective();

private:
  void setupLog();
  void setupDockManager();
  void setupMenuBar();
  void createConnectionUi();
  void createPerspectiveUi();
  void createCentralUI();
  void createDeviceTableUi();
  void createProgressWindowUi();
  void loadPreferences();

  void scanLocalNetwork();
  void addDevice(Device device);
  void addLocalHostDevice(Device device);
  void removeDevice(Device device);
  void removeDevices();
  void createProgressWindow();

  Ui::FlowLink *ui;

  /* network */
  QTimer *m_scanLocalNetworkTimer = nullptr;
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
  QAction *m_toggleShowLocalHostAction = nullptr;

  // perspective
  QMenu *m_perspectivesMenu = nullptr;
  QToolButton *m_perspectivesToolBtn = nullptr;
  QAction *m_newPerspectiveAction = nullptr;
  QAction *m_saveAsCurrentPerspectiveAction = nullptr;
  QAction *m_deletePerspectiveAction = nullptr;
  QWidgetAction *m_perspectiveListAction = nullptr;
  QAction *m_setDefaultPerspective = nullptr;
  QComboBox *m_perspectiveComboBox = nullptr;

  /* dockwidget */
  QLabel *centralDockLabel = nullptr;
  TableModel *m_deviceTableModel = nullptr;
  QSortFilterProxyModel *m_deviceProxyModel = nullptr;
  QTableView *m_deviceTableView = nullptr;
  ProgressWindow *m_progressWindow = nullptr;

  /* fields*/
  std::unique_ptr<std::unordered_map<QString, ChatWindow *>> m_chatWindowMap = nullptr; // a map used to store chat window pointers
  QString m_currentChatWindowAddress;
  Device m_localHostDevice;

  /* flags */
  bool m_isShowLocalHost;
};
#endif // FLOWLINK_H
