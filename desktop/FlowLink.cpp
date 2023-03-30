#include <QHeaderView>

#include "./ui_FlowLink.h"
#include "FlowLink.h"

using namespace ads;

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::FlowLink), receiver(new Receiver)
{
  ui->setupUi(this);

  // dock manager
  setupDockManager();

  // toolbar
  createConnectionUi();
  createPerspectiveUi();

  // central widget
  createCentralUI();

  // device table widget
  createDeviceTableUi();

  // properties table widget
  createPropertiesTableUi();
}

FlowLink::~FlowLink()
{
  delete ui;
}

void FlowLink::setupDockManager()
{
  CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
  CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
  CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
  CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
  dockManager = new CDockManager(this);
}

void FlowLink::createConnectionUi()
{
  connectAction = new QAction("Connect", this);
  connect(
      connectAction, &QAction::triggered, receiver, &Receiver::createConnection);
  connect(
      receiver, &Receiver::sendHostInfo, [&](Host host)
      { addDevice(host); });
  ui->toolBar->addAction(connectAction);
}

void FlowLink::createPerspectiveUi()
{
  savePerspectiveAction = new QAction("Create Perspective", this);
  connect(savePerspectiveAction, SIGNAL(triggered()), SLOT(savePerspective()));
  perspectiveListAction = new QWidgetAction(this);
  perspectiveComboBox = new QComboBox(this);
  perspectiveComboBox->setSizeAdjustPolicy(QComboBox::AdjustToContents);
  perspectiveComboBox->setSizePolicy(QSizePolicy::Preferred,
                                     QSizePolicy::Preferred);
  connect(perspectiveComboBox,
          SIGNAL(currentTextChanged(const QString &)),
          dockManager,
          SLOT(openPerspective(const QString &)));
  perspectiveListAction->setDefaultWidget(perspectiveComboBox);
  ui->toolBar->addSeparator();
  ui->toolBar->addAction(perspectiveListAction);
  ui->toolBar->addAction(savePerspectiveAction);
}

void FlowLink::createCentralUI()
{
  QLabel *l = new QLabel();
  l->setText("Welcome");
  l->setAlignment(Qt::AlignCenter);
  CDockWidget *centralDockWidget = new CDockWidget("Central Widget");
  centralDockWidget->setWidget(l);
  centralDockArea = dockManager->setCentralWidget(centralDockWidget);
  centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);
}

void FlowLink::createDeviceTableUi()
{
  deviceTableModel = new TableModel();

  deviceProxyModel = new QSortFilterProxyModel();
  deviceProxyModel->setSourceModel(deviceTableModel);

  deviceTableView = new QTableView();
  deviceTableView->setModel(deviceProxyModel);
  deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
  deviceTableView->horizontalHeader()->setSectionResizeMode(
      QHeaderView::ResizeToContents);
  deviceTableView->setSortingEnabled(true);

  CDockWidget *deviceDockWidget = new CDockWidget("Devices");
  deviceDockWidget->setWidget(deviceTableView);
  deviceDockWidget->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromDockWidget);
  deviceDockWidget->setMinimumSize(200, 150);
  dockManager->addDockWidget(
      DockWidgetArea::TopDockWidgetArea, deviceDockWidget, centralDockArea);
  ui->menuView->addAction(deviceDockWidget->toggleViewAction());
}

void FlowLink::createPropertiesTableUi()
{
  QTableWidget *propertiesTable = new QTableWidget();
  propertiesTable->setColumnCount(3);
  propertiesTable->setRowCount(10);
  CDockWidget *propertiesDockWidget = new CDockWidget("Properties");
  propertiesDockWidget->setWidget(propertiesTable);
  propertiesDockWidget->setMinimumSizeHintMode(
      CDockWidget::MinimumSizeHintFromDockWidget);
  propertiesDockWidget->resize(200, 150);
  propertiesDockWidget->setMinimumSize(200, 150);
  dockManager->addDockWidget(
      DockWidgetArea::RightDockWidgetArea, propertiesDockWidget, centralDockArea);
  ui->menuView->addAction(propertiesDockWidget->toggleViewAction());
}

void FlowLink::addDevice(Host host)
{
  deviceTableModel->addRow(host.name, host.address);
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
}

void FlowLink::closeEvent(QCloseEvent *event)
{
  // To delete all floating widgets, remove the dock manager.
  // This will ensure that all top-level windows associated with the dock
  // manager are closed correctly.
  dockManager->deleteLater();
  QMainWindow::closeEvent(event);
}