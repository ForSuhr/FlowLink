#include <QLabel>
#include <QInputDialog>
#include <QDebug>

#include "FlowLink.h"
#include "./ui_FlowLink.h"

using namespace ads;

typedef enum DeviceTableHorizontalLabel {
    Type, 
    Name, 
    Address, 
    Note
} DTHL;

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::FlowLink),
      receiver(new Receiver)
{
    ui->setupUi(this);

    // dock manager
    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
    CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
    dockManager = new CDockManager(this);

    // toolbar
    createConnectionUi();
    createPerspectiveUi();

    // central widget
    QLabel *l = new QLabel();
    l->setText("Welcome");
    l->setAlignment(Qt::AlignCenter);
    CDockWidget *centralDockWidget = new CDockWidget("Central Widget");
    centralDockWidget->setWidget(l);
    auto *centralDockArea = dockManager->setCentralWidget(centralDockWidget);
    centralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    // device table widget
    deviceTable = new QTableWidget();
    deviceTable->setColumnCount(4);
    deviceTable->setHorizontalHeaderLabels(QStringList({"Type", "Name", "Address", "Note"}));
    CDockWidget *deviceDockWidget = new CDockWidget("Device List");
    deviceDockWidget->setWidget(deviceTable);
    deviceDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    deviceDockWidget->setMinimumSize(200, 150);
    dockManager->addDockWidget(DockWidgetArea::TopDockWidgetArea, deviceDockWidget, centralDockArea);
    ui->menuView->addAction(deviceDockWidget->toggleViewAction());

    // properties table widget
    QTableWidget *propertiesTable = new QTableWidget();
    propertiesTable->setColumnCount(3);
    propertiesTable->setRowCount(10);
    CDockWidget *propertiesDockWidget = new CDockWidget("Properties");
    propertiesDockWidget->setWidget(propertiesTable);
    propertiesDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
    propertiesDockWidget->resize(200, 150);
    propertiesDockWidget->setMinimumSize(200, 150);
    dockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, propertiesDockWidget, centralDockArea);
    ui->menuView->addAction(propertiesDockWidget->toggleViewAction());
}

FlowLink::~FlowLink()
{
    delete ui;
}

void FlowLink::createConnectionUi()
{
    connectAction = new QAction("Connect", this);
    connect(connectAction, &QAction::triggered, receiver, &Receiver::createConnection);
    connect(receiver, &Receiver::sendHostInfo, [&](Host host)
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
    perspectiveComboBox->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    connect(perspectiveComboBox, SIGNAL(currentTextChanged(const QString &)),
            dockManager, SLOT(openPerspective(const QString &)));
    perspectiveListAction->setDefaultWidget(perspectiveComboBox);
    ui->toolBar->addSeparator();
    ui->toolBar->addAction(perspectiveListAction);
    ui->toolBar->addAction(savePerspectiveAction);
}

void FlowLink::addDevice(Host host)
{
    deviceList.push_back(host);
    QTableWidgetItem* item = new QTableWidgetItem();
    deviceTable->setItem(0, DTHL::Type);
    // QStringListModel *model = new QStringListModel(this);
    // QStringList list;
    // // TODO: add the real host name and address
    // list << QString("Host Name: %1  Host Address: %2").arg(host.name).arg(host.address);
    // model->setStringList(list);
    // deviceTable->setModel(model);
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
    // This will ensure that all top-level windows associated with the dock manager are closed correctly.
    dockManager->deleteLater();
    QMainWindow::closeEvent(event);
}