#include "./ui_FlowLink.h"
#include "FlowLink.h"

using namespace ads;

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::FlowLink),
      m_udpReceiver(new UdpReceiver),
      m_udpSender(new UdpSender)
{
  setupLog();

  ui->setupUi(this);

  setupDockManager();

  /* toolbar */
  createConnectionActionUi();
  createChatActionUi();
  createPerspectiveUi();

  /* central widget */
  createCentralUI();

  /* device table widget */
  createDeviceTableUi();

  /* properties table widget */
  createPropertiesTableUi();

  /* load preferences */
  loadPreferences();
}

FlowLink::~FlowLink()
{
  delete ui;
  PLOG_DEBUG << "----- Stop Logging -----";
}

void FlowLink::setupLog()
{
  static plog::ConsoleAppender<plog::TxtFormatter> consoleAppender;
  plog::init(plog::debug, &consoleAppender);
  PLOG_DEBUG << "----- Start Logging -----";
}

void FlowLink::setupDockManager()
{
  CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
  CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
  CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
  CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
  m_dockManager = new CDockManager(this);
}

void FlowLink::createConnectionActionUi()
{
  m_connectAction = new QAction("Connect", this);
  m_disconnectAction = new QAction("Disconnect", this);
  m_connectAction->setEnabled(true);
  m_disconnectAction->setEnabled(false);

  // connect
  ui->toolBar->addAction(m_connectAction);
  connect(m_connectAction, &QAction::triggered, this, &FlowLink::onConnectActionClicked);
  connect(m_udpReceiver, &UdpReceiver::sendDeviceInfo, [&](Device device, DeviceAction deviceAction)
          { if (deviceAction == DeviceAction::Connection) {addDevice(device);} });

  // disconnect
  ui->toolBar->addAction(m_disconnectAction);
  connect(m_disconnectAction, &QAction::triggered, this, &FlowLink::onDisconnectActionClicked);
  connect(m_udpReceiver, &UdpReceiver::sendDeviceInfo, [&](Device device, DeviceAction deviceAction)
          {if (deviceAction == DeviceAction::Disconnection) {removeDevices();} });
}

void FlowLink::createChatActionUi()
{
  m_chatAction = new QAction("Chat", this);
  ui->toolBar->addSeparator();
  ui->toolBar->addAction(m_chatAction);
  connect(m_chatAction, &QAction::triggered, this, &FlowLink::onChatActionClicked);
}

void FlowLink::createPerspectiveUi()
{
  m_savePerspectiveAction = new QAction("Create Perspective", this);
  connect(m_savePerspectiveAction, SIGNAL(triggered()), this, SLOT(savePerspective()));

  m_perspectiveListAction = new QWidgetAction(this);
  m_perspectiveComboBox = new QComboBox(this);
  m_perspectiveComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  m_perspectiveComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  connect(m_perspectiveComboBox, SIGNAL(currentTextChanged(const QString &)), m_dockManager, SLOT(openPerspective(const QString &)));
  m_perspectiveListAction->setDefaultWidget(m_perspectiveComboBox);

  m_setDefaultPerspective = new QAction("Set Default Perspective", this);
  connect(m_setDefaultPerspective, &QAction::triggered, [&]
          { if (m_perspectiveComboBox->count()!=0)
            config.setValue("Perspective", m_perspectiveComboBox->currentText()); });

  ui->toolBar->addSeparator();
  ui->toolBar->addAction(m_perspectiveListAction);
  ui->toolBar->addAction(m_savePerspectiveAction);
  ui->toolBar->addAction(m_setDefaultPerspective);
}

void FlowLink::createCentralUI()
{
  m_centralDockWidget = new CDockWidget("Chat");
  QLabel *centralDockLabel = new QLabel("Flow Link");
  centralDockLabel->setAlignment(Qt::AlignCenter);
  m_centralDockWidget->setWidget(centralDockLabel);
  m_centralDockArea = m_dockManager->setCentralWidget(m_centralDockWidget);
  m_centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);
  ui->menuView->addAction(m_centralDockWidget->toggleViewAction());
}

void FlowLink::createDeviceTableUi()
{
  m_deviceTableModel = new TableModel();

  m_deviceProxyModel = new QSortFilterProxyModel();
  m_deviceProxyModel->setSourceModel(m_deviceTableModel);

  m_deviceTableView = new QTableView();
  m_deviceTableView->setModel(m_deviceProxyModel);
  m_deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_deviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  m_deviceTableView->setSortingEnabled(true);

  CDockWidget *deviceDockWidget = new CDockWidget("Devices");
  deviceDockWidget->setWidget(m_deviceTableView);
  deviceDockWidget->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromDockWidget);
  deviceDockWidget->setMinimumSize(200, 150);
  m_dockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea, deviceDockWidget, m_centralDockArea);
  ui->menuView->addAction(deviceDockWidget->toggleViewAction());
}

void FlowLink::createPropertiesTableUi()
{
  m_propertiesTable = new QTableWidget();
  m_propertiesTable->setColumnCount(1);
  m_propertiesTable->setRowCount(10);
  CDockWidget *propertiesDockWidget = new CDockWidget("Properties");
  propertiesDockWidget->setWidget(m_propertiesTable);
  propertiesDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
  propertiesDockWidget->resize(200, 150);
  propertiesDockWidget->setMinimumSize(200, 150);
  m_dockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, propertiesDockWidget, m_centralDockArea);
  ui->menuView->addAction(propertiesDockWidget->toggleViewAction());
}

void FlowLink::loadPreferences()
{
  /*  load perspectives from local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  m_dockManager->loadPerspectives(settings);
  m_perspectiveComboBox->clear();
  m_perspectiveComboBox->addItems(m_dockManager->perspectiveNames());
  m_perspectiveComboBox->setCurrentText(config.value("Perspective").toString());
}

void FlowLink::onConnectActionClicked()
{
  m_udpReceiver->createConnection();
  m_udpSender->sendDeviceInfo();

  m_disconnectAction->setEnabled(true);
}

void FlowLink::onDisconnectActionClicked()
{
  m_udpReceiver->closeConnection();
  m_disconnectAction->setEnabled(false);
}

/// @brief open a chat window of the selected device
void FlowLink::onChatActionClicked()
{
  // get indexes from the selected part of the table
  QItemSelectionModel *selectionModel = m_deviceTableView->selectionModel();
  const QModelIndexList indexes = selectionModel->selectedRows(TableHeader::Address);

  // if a part of the table is selected, pick the first item as the to be connected device, open a chat window for it
  if (!indexes.isEmpty())
  {
    // get the address of the to be connected device
    QModelIndex index = indexes.first();
    QString address = index.data().toString();

    // get chat window by address<QString>
    ChatWindow *chatWindow = m_chatWindowMap[address];

    // set it as central widget
    m_centralDockWidget->setWidget(chatWindow);
  }
}

/// @brief add device to the table view and create a chat window for it
/// @param the to be added device
void FlowLink::addDevice(Device device)
{
  m_deviceTableModel->addRow(device.name, device.address);

  // if the chat window map does not contain a chat window of this device,
  // create a new chat window and store its pointer by address string in the map
  if (m_chatWindowMap.find(device.address) == m_chatWindowMap.end())
  {
    ChatWindow *chatWindow = new ChatWindow(device.address);
    m_chatWindowMap[device.address] = chatWindow;
  }
}

/// @brief remove all devices from the table view and delete all chat windows
void FlowLink::removeDevices()
{
  // remove all devices from the table
  int rowNum = m_deviceTableModel->rowCount(QModelIndex());
  m_deviceTableModel->removeRows(0, rowNum, QModelIndex());

  // clear the chat window map
  m_chatWindowMap.clear();
}

void FlowLink::savePerspective()
{
  QString perspectiveName = QInputDialog::getText(this, "Save Perspective", "Perspective Name:");
  if (perspectiveName.isEmpty())
  {
    return;
  }

  m_dockManager->addPerspective(perspectiveName);
  QSignalBlocker Blocker(m_perspectiveComboBox);
  m_perspectiveComboBox->clear();
  m_perspectiveComboBox->addItems(m_dockManager->perspectiveNames());
  m_perspectiveComboBox->setCurrentText(perspectiveName);

  /*  save perspectives as local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  m_dockManager->savePerspectives(settings);
}

void FlowLink::closeEvent(QCloseEvent *event)
{
  // To delete all floating widgets, remove the dock manager.
  // This will ensure that all top-level windows associated with the dock
  // manager are closed correctly.
  m_dockManager->deleteLater();
  QMainWindow::closeEvent(event);
}