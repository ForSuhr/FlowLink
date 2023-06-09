#include "./ui_FlowLink.h"
#include "FlowLink.h"

using namespace ads;

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::FlowLink),
      udpReceiver(new UdpReceiver),
      udpSender(new UdpSender),
      tcpReceiver(new TcpReceiver)
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
  dockManager = new CDockManager(this);
}

void FlowLink::createConnectionActionUi()
{
  connectAction = new QAction("Connect", this);
  disconnectAction = new QAction("Disconnect", this);
  connectAction->setEnabled(true);
  disconnectAction->setEnabled(false);

  // connect
  ui->toolBar->addAction(connectAction);
  connect(connectAction, &QAction::triggered, this, &FlowLink::onConnectActionClicked);
  connect(udpReceiver, &UdpReceiver::sendDeviceInfo, [&](Device device, DeviceAction deviceAction)
          { if (deviceAction == DeviceAction::Connection) {addDevice(device);} });

  // disconnect
  ui->toolBar->addAction(disconnectAction);
  connect(disconnectAction, &QAction::triggered, this, &FlowLink::onDisconnectActionClicked);
  connect(udpReceiver, &UdpReceiver::sendDeviceInfo, [&](Device device, DeviceAction deviceAction)
          {if (deviceAction == DeviceAction::Disconnection) {removeDevices();} });
}

void FlowLink::createChatActionUi()
{
  chatAction = new QAction("Chat", this);
  ui->toolBar->addSeparator();
  ui->toolBar->addAction(chatAction);
  connect(chatAction, &QAction::triggered, this, &FlowLink::onChatActionClicked);
}

void FlowLink::createPerspectiveUi()
{
  savePerspectiveAction = new QAction("Create Perspective", this);
  connect(savePerspectiveAction, SIGNAL(triggered()), this, SLOT(savePerspective()));

  perspectiveListAction = new QWidgetAction(this);
  perspectiveComboBox = new QComboBox(this);
  perspectiveComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  perspectiveComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  connect(perspectiveComboBox, SIGNAL(currentTextChanged(const QString &)), dockManager, SLOT(openPerspective(const QString &)));
  perspectiveListAction->setDefaultWidget(perspectiveComboBox);

  setDefaultPerspective = new QAction("Set Default Perspective", this);
  connect(setDefaultPerspective, &QAction::triggered, [&]
          { if (NULL != perspectiveComboBox->count())
            config.setValue("Perspective", perspectiveComboBox->currentText()); });

  ui->toolBar->addSeparator();
  ui->toolBar->addAction(perspectiveListAction);
  ui->toolBar->addAction(savePerspectiveAction);
  ui->toolBar->addAction(setDefaultPerspective);
}

void FlowLink::createCentralUI()
{
  createChatWindowUi();

  CDockWidget *centralDockWidget = new CDockWidget("Chat Window");
  centralDockWidget->setWidget(chatWindow);
  centralDockArea = dockManager->setCentralWidget(centralDockWidget);
  centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);
  ui->menuView->addAction(centralDockWidget->toggleViewAction());
}

void FlowLink::createChatWindowUi()
{
  chatWindow = new ChatWindow();
  connect(chatWindow, &ChatWindow::onBtnSendClickedSignal, [&]()
          { QString msg = chatWindow->msgText();
            tcpSender->sendMsg(msg);
            appendTextToChatWindow(chatWindow, msg); });
  connect(tcpReceiver, &TcpReceiver::msgSignal, [&](const QString &msg)
          { appendTextToChatWindow(chatWindow, msg); });
}

void FlowLink::createDeviceTableUi()
{
  deviceTableModel = new TableModel();

  deviceProxyModel = new QSortFilterProxyModel();
  deviceProxyModel->setSourceModel(deviceTableModel);

  deviceTableView = new QTableView();
  deviceTableView->setModel(deviceProxyModel);
  deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  deviceTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  deviceTableView->setSortingEnabled(true);

  CDockWidget *deviceDockWidget = new CDockWidget("Devices");
  deviceDockWidget->setWidget(deviceTableView);
  deviceDockWidget->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromDockWidget);
  deviceDockWidget->setMinimumSize(200, 150);
  dockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea, deviceDockWidget, centralDockArea);
  ui->menuView->addAction(deviceDockWidget->toggleViewAction());
}

void FlowLink::createPropertiesTableUi()
{
  propertiesTable = new QTableWidget();
  propertiesTable->setColumnCount(1);
  propertiesTable->setRowCount(10);
  CDockWidget *propertiesDockWidget = new CDockWidget("Properties");
  propertiesDockWidget->setWidget(propertiesTable);
  propertiesDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
  propertiesDockWidget->resize(200, 150);
  propertiesDockWidget->setMinimumSize(200, 150);
  dockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, propertiesDockWidget, centralDockArea);
  ui->menuView->addAction(propertiesDockWidget->toggleViewAction());
}

void FlowLink::loadPreferences()
{
  /*  load perspectives from local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  dockManager->loadPerspectives(settings);
  perspectiveComboBox->clear();
  perspectiveComboBox->addItems(dockManager->perspectiveNames());
  perspectiveComboBox->setCurrentText(config.value("Perspective").toString());
}

void FlowLink::onConnectActionClicked()
{
  udpReceiver->createConnection();
  udpSender->sendDeviceInfo();

  disconnectAction->setEnabled(true);
}

void FlowLink::onDisconnectActionClicked()
{
  udpReceiver->closeConnection();
  disconnectAction->setEnabled(false);
}

void FlowLink::onChatActionClicked()
{
  QItemSelectionModel *selectionModel = deviceTableView->selectionModel();
  const QModelIndexList indexes = selectionModel->selectedRows(TableHeader::Address);
  if (!indexes.isEmpty())
  {
    QModelIndex index = indexes.first();
    QString address = index.data().toString();
    appendTextToChatWindow(chatWindow, address);

    tcpSender = new TcpSender(address);
  }
}

void FlowLink::addDevice(Device device)
{
  deviceTableModel->addRow(device.name, device.address);
}

void FlowLink::removeDevices()
{
  int rowNum = deviceTableModel->rowCount(QModelIndex());
  deviceTableModel->removeRows(0, rowNum, QModelIndex());
}

void FlowLink::savePerspective()
{
  QString perspectiveName = QInputDialog::getText(this, "Save Perspective", "Perspective Name:");
  if (perspectiveName.isEmpty())
  {
    return;
  }

  dockManager->addPerspective(perspectiveName);
  QSignalBlocker Blocker(perspectiveComboBox);
  perspectiveComboBox->clear();
  perspectiveComboBox->addItems(dockManager->perspectiveNames());
  perspectiveComboBox->setCurrentText(perspectiveName);

  /*  save perspectives as local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  dockManager->savePerspectives(settings);
}

void FlowLink::closeEvent(QCloseEvent *event)
{
  // To delete all floating widgets, remove the dock manager.
  // This will ensure that all top-level windows associated with the dock
  // manager are closed correctly.
  dockManager->deleteLater();
  QMainWindow::closeEvent(event);
}