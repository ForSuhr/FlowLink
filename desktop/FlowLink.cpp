#include "./ui_FlowLink.h"
#include "FlowLink.h"

using namespace ads;

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::FlowLink),
      m_udpReceiver(new UdpReceiver),
      m_udpSender(new UdpSender),
      m_chatWindowMap(new std::unordered_map<QString, ChatWindow *>)
{
  setupLog();

  ui->setupUi(this);

  setupDockManager();

  /* menubar */
  setupMenuBar();

  /* toolbar */
  createConnectionUi();
  createPerspectiveUi();

  /* central widget */
  createCentralUI();

  /* device table widget */
  createDeviceTableUi();

  /* progress table widget */
  createProgressWindowUi();

  /* load preferences */
  loadPreferences();

  PLOG_DEBUG << "running on main thread: " << QThread::currentThread();
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

void FlowLink::setupMenuBar()
{
  /* file menu */
  // exit
  ui->menuFile->addSeparator();
  QAction *exitAction = new QAction(tr("Exit"), this);
  ui->menuFile->addAction(exitAction);
  connect(exitAction, &QAction::triggered, qApp, &QCoreApplication::quit, Qt::QueuedConnection);

  /* edit menu */
  // preferences
  QAction *preferencesAction = new QAction(tr("Preferences"), this);
  ui->menuEdit->addAction(preferencesAction);
  prefWindow = new PrefWindow(this);
  prefWindow->setWindowFlag(Qt::Window);
  prefWindow->setWindowModality(Qt::WindowModal);
  prefWindow->resize(prefWindow->width(), 300);
  connect(preferencesAction, &QAction::triggered, prefWindow, &PrefWindow::show);
}

void FlowLink::createConnectionUi()
{
  m_scanLocalNetworkTimer = new QTimer(this);
  m_connectAction = new QAction(this);
  m_connectAction->setIcon(QIcon(R"(:/asset/style/lumos/connectAction.svg)"));
  m_connectAction->setToolTip(tr("Connect to local network"));
  m_connectAction->setEnabled(true);
  m_disconnectAction = new QAction(this);
  m_disconnectAction->setIcon(QIcon(R"(:/asset/style/lumos/disconnectAction.svg)"));
  m_disconnectAction->setToolTip(tr("Disonnect from local network"));
  m_disconnectAction->setEnabled(false);
  m_toggleShowLocalHostAction = new QAction(this);
  m_toggleShowLocalHostAction->setIcon(QIcon(R"(:/asset/style/lumos/hideLocalHost.svg)"));
  m_toggleShowLocalHostAction->setToolTip(tr("Show/Hide local host"));

  // a timer for scanning devices in local network
  connect(m_scanLocalNetworkTimer, &QTimer::timeout, this, &FlowLink::scanLocalNetwork);

  // connect
  ui->toolBar->addAction(m_connectAction);
  connect(m_connectAction, &QAction::triggered, this, &FlowLink::onConnectActionClicked);
  connect(m_udpReceiver, &UdpReceiver::receivedDeviceInfo, [&](Device device, DeviceAction deviceAction)
          {if (deviceAction == DeviceAction::Connection) {addDevice(device);} });
  connect(m_udpReceiver, &UdpReceiver::receivedDeviceInfo, [&](Device device, DeviceAction deviceAction)
          {if (deviceAction == DeviceAction::LocalHostConnection){addLocalHostDevice(device);} });

  // disconnect
  ui->toolBar->addAction(m_disconnectAction);
  connect(m_disconnectAction, &QAction::triggered, this, &FlowLink::onDisconnectActionClicked);
  connect(m_udpReceiver, &UdpReceiver::receivedDeviceInfo, [&](Device device, DeviceAction deviceAction)
          {if (deviceAction == DeviceAction::Disconnection){removeDevices();} });

  // show local host
  ui->toolBar->addAction(m_toggleShowLocalHostAction);
  connect(m_toggleShowLocalHostAction, &QAction::triggered, this, &FlowLink::onToggleShowLocalHostActionClicked);
}

void FlowLink::createPerspectiveUi()
{
  m_newPerspectiveAction = new QAction(tr("New Perspective"), this);
  connect(m_newPerspectiveAction, SIGNAL(triggered()), this, SLOT(NewPerspective()));
  m_saveAsCurrentPerspectiveAction = new QAction(tr("Save As Current Perspective"), this);
  connect(m_saveAsCurrentPerspectiveAction, SIGNAL(triggered()), this, SLOT(SaveAsCurrentPerspective()));
  m_deletePerspectiveAction = new QAction(tr("Delete Current Perspective"), this);
  connect(m_deletePerspectiveAction, SIGNAL(triggered()), this, SLOT(deletePerspective()));

  m_perspectivesMenu = new QMenu(this);
  m_perspectivesToolBtn = new QToolButton(this);
  m_perspectivesToolBtn->setIcon(QIcon(R"(:/asset/style/lumos/perspectivesAction.svg)"));
  m_perspectivesToolBtn->setPopupMode(QToolButton::InstantPopup);
  m_perspectiveListAction = new QWidgetAction(this);
  m_perspectiveComboBox = new QComboBox(this);
  m_perspectiveComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  m_perspectiveComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
  m_perspectiveComboBox->setFixedHeight(24);
  connect(m_perspectiveComboBox, SIGNAL(currentTextChanged(const QString &)), m_dockManager, SLOT(openPerspective(const QString &)));
  m_perspectiveListAction->setDefaultWidget(m_perspectiveComboBox);

  m_setDefaultPerspective = new QAction(tr("Set Default Perspective"), this);
  connect(m_setDefaultPerspective, &QAction::triggered, [&]
          {
    if (m_perspectiveComboBox->count() != 0)
      config.setValue("Perspective", m_perspectiveComboBox->currentText()); });

  // add a stretchable spacer beforehand, using an empty QWidget because QToolBar doesn't provide addStretch()
  QWidget *spacer = new QWidget();
  spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
  ui->toolBar->addWidget(spacer);

  // add perspective-actions
  ui->toolBar->addWidget(m_perspectivesToolBtn);
  m_perspectivesToolBtn->setMenu(m_perspectivesMenu);
  m_perspectivesMenu->addAction(m_setDefaultPerspective);
  m_perspectivesMenu->addAction(m_newPerspectiveAction);
  m_perspectivesMenu->addAction(m_saveAsCurrentPerspectiveAction);
  m_perspectivesMenu->addAction(m_deletePerspectiveAction);
  ui->toolBar->addAction(m_perspectiveListAction);
}

void FlowLink::createCentralUI()
{
  m_centralDockWidget = new CDockWidget("Chat");
  centralDockLabel = new QLabel("Flow Link");
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

  CDockWidget *deviceDockWidget = new CDockWidget(tr("Devices"));
  deviceDockWidget->setWidget(m_deviceTableView);
  deviceDockWidget->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromDockWidget);
  deviceDockWidget->setMinimumSize(200, 150);
  m_dockManager->addDockWidget(DockWidgetArea::LeftDockWidgetArea, deviceDockWidget, m_centralDockArea);
  ui->menuView->addAction(deviceDockWidget->toggleViewAction());

  // connect signal-slot for clicking on a device in the devices table
  connect(m_deviceTableView, &QTableView::clicked, this, &FlowLink::openChatWindow);
}

void FlowLink::createProgressWindowUi()
{
  m_progressWindow = new ProgressWindow();
  CDockWidget *progressDockWidget = new CDockWidget(tr("Progress"));
  progressDockWidget->setWidget(m_progressWindow);
  progressDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
  progressDockWidget->resize(200, 150);
  progressDockWidget->setMinimumSize(200, 150);
  m_dockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, progressDockWidget, m_centralDockArea);
  ui->menuView->addAction(progressDockWidget->toggleViewAction());
}

void FlowLink::loadPreferences()
{
  /*  load perspectives from local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  m_dockManager->loadPerspectives(settings);
  m_perspectiveComboBox->clear();
  m_perspectiveComboBox->addItems(m_dockManager->perspectiveNames());
  m_perspectiveComboBox->setCurrentText(config.value("Perspective").toString());

  /* show local host */
  m_isShowLocalHost = config.value("common/showLocalHost").toBool();
  if (m_isShowLocalHost)
    m_toggleShowLocalHostAction->setIcon(QIcon(R"(:/asset/style/lumos/showLocalHost.svg)"));
  else
    m_toggleShowLocalHostAction->setIcon(QIcon(R"(:/asset/style/lumos/hideLocalHost.svg)"));

  /* load stylesheet */
  QMap<QString, QString> stylesheetMap;
  setUpStylesheetMap(stylesheetMap);
  StyleSheet::Instance().loadQSS(this, stylesheetMap["Lumos"]);
}

void FlowLink::onConnectActionClicked()
{
  m_udpReceiver->createConnection();
  scanLocalNetwork();
  m_scanLocalNetworkTimer->start(2000);

  m_connectAction->setEnabled(false);
  m_disconnectAction->setEnabled(true);
}

void FlowLink::onDisconnectActionClicked()
{
  m_scanLocalNetworkTimer->stop();

  /* close tcp connection */
  for (const auto &pair : (*m_chatWindowMap))
  {
    pair.second->m_tcpReceiver->closeConnection();
  }

  /* close udp connection and clear chatwindow */
  m_udpReceiver->closeConnection();

  /* ui */
  m_connectAction->setEnabled(true);
  m_disconnectAction->setEnabled(false);
}

void FlowLink::onToggleShowLocalHostActionClicked()
{
  m_isShowLocalHost = !m_isShowLocalHost;
  if (m_isShowLocalHost)
  {
    m_toggleShowLocalHostAction->setIcon(QIcon(R"(:/asset/style/lumos/showLocalHost.svg)"));
    config.setValue("common/showLocalHost", true);
    if (m_chatWindowMap->find(m_localHostDevice.address) != m_chatWindowMap->end())
      addDevice(m_localHostDevice);
  }
  else
  {
    m_toggleShowLocalHostAction->setIcon(QIcon(R"(:/asset/style/lumos/hideLocalHost.svg)"));
    config.setValue("common/showLocalHost", false);
    removeDevice(m_localHostDevice);
  }
}

/// @brief open a chat window of the selected device
void FlowLink::openChatWindow()
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
    ChatWindow *chatWindow = (*m_chatWindowMap)[address];
    m_currentChatWindowAddress = address;

    // set it as central widget
    m_centralDockWidget->setWidget(chatWindow);
  }
}

void FlowLink::scanLocalNetwork()
{
  m_udpSender->sendDeviceInfo();
}

/// @brief add device to the table view and create a chat window for it
/// @param the to be added device
void FlowLink::addDevice(Device device)
{
  m_deviceTableModel->addRow(device.name, device.address);

  // if the chat window map does not contain a chat window of this device,
  if (m_chatWindowMap->find(device.address) == m_chatWindowMap->end())
  {
    // create a new chat window and store its pointer by address string in the map
    ChatWindow *chatWindow = new ChatWindow(device.address, device.port);
    (*m_chatWindowMap)[device.address] = chatWindow;

    // create a connection to notify the progress window that there is a new download task
    connect((*m_chatWindowMap)[device.address]->m_tcpReceiver, &TcpReceiver::startNewTaskSignal, m_progressWindow, &ProgressWindow::createProgressWidget);
    // create a connection to bind progress-update signal to progress widgets
    connect((*m_chatWindowMap)[device.address]->m_tcpReceiver, &TcpReceiver::updateProgressSignal, m_progressWindow, &ProgressWindow::updateProgress);
  }
}

void FlowLink::addLocalHostDevice(Device device)
{
  m_localHostDevice = device;
  addDevice(device);
  if (!m_isShowLocalHost)
    removeDevice(device);
}

void FlowLink::removeDevice(Device device)
{
  // remove the given device from table
  m_deviceTableModel->removeRow(device.name, device.address);
}

/// @brief remove all devices from the table view and delete all chat windows
void FlowLink::removeDevices()
{
  // remove all devices from the table
  int rowNum = m_deviceTableModel->rowCount(QModelIndex());
  m_deviceTableModel->removeRows(0, rowNum, QModelIndex());

  // clear the chat window map
  for (auto &i : (*m_chatWindowMap))
  {
    i.second->deleteLater();
  }
  m_chatWindowMap->clear();
  m_centralDockWidget->setWidget(centralDockLabel);
}

void FlowLink::NewPerspective()
{
  QString perspectiveName = QInputDialog::getText(this, tr("New Perspective"), tr("Perspective Name:"));
  if (perspectiveName.isEmpty())
  {
    return;
  }

  m_dockManager->addPerspective(perspectiveName);
  QSignalBlocker Blocker(m_perspectiveComboBox);
  m_perspectiveComboBox->clear();
  m_perspectiveComboBox->addItems(m_dockManager->perspectiveNames());
  m_perspectiveComboBox->setCurrentText(perspectiveName);

  /*  update current perspectives in combobox to local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  m_dockManager->savePerspectives(settings);
}

void FlowLink::SaveAsCurrentPerspective()
{
  QString perspectiveName = m_perspectiveComboBox->currentText();

  m_dockManager->addPerspective(perspectiveName);
  QSignalBlocker Blocker(m_perspectiveComboBox);
  m_perspectiveComboBox->clear();
  m_perspectiveComboBox->addItems(m_dockManager->perspectiveNames());
  m_perspectiveComboBox->setCurrentText(perspectiveName);

  /*  update current perspectives in combobox to local file */
  QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
  m_dockManager->savePerspectives(settings);
}

void FlowLink::deletePerspective()
{
  QString perspectiveName = m_perspectiveComboBox->currentText();
  if (m_perspectiveComboBox->count() == 1)
    QMessageBox::warning(this, "", tr("Can not delete last perspective!"));
  else
  {
    m_dockManager->removePerspective(perspectiveName);
    m_perspectiveComboBox->clear();
    m_perspectiveComboBox->addItems(m_dockManager->perspectiveNames());
    m_perspectiveComboBox->setCurrentIndex(0);

    /*  update current perspectives in combobox to local file */
    QSettings settings("./config/perspectives.ini", QSettings::IniFormat);
    m_dockManager->savePerspectives(settings);
  }
}

void FlowLink::closeEvent(QCloseEvent *event)
{
  // To delete all floating widgets, remove the dock manager.
  // This will ensure that all top-level windows associated with the dock
  // manager are closed correctly.
  m_dockManager->deleteLater();
  QMainWindow::closeEvent(event);
}