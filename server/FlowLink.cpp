#include <QLabel>
#include <QListView>

#include "FlowLink.h"
#include "./ui_FlowLink.h"

using namespace ads;

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlowLink)
{
    ui->setupUi(this);

//    CDockManager::setConfigFlag(CDockManager::OpaqueSplitterResize, true);
//    CDockManager::setConfigFlag(CDockManager::XmlCompressionEnabled, false);
//    CDockManager::setConfigFlag(CDockManager::FocusHighlighting, true);
//    CDockManager::setAutoHideConfigFlags(CDockManager::DefaultAutoHideConfig);
    dockManager = new CDockManager(this);
//    statusDockArea;
//    timelineDockWidget;

    // Set central widget
//    QLabel* l = new QLabel();
//    l->setText("Welcome");
//    CDockWidget* centralDockWidget = new CDockWidget("Central Widget");
//    centralDockWidget->setWidget(l);
//    auto* CentralDockArea = dockManager->setCentralWidget(centralDockWidget);
//    CentralDockArea->setAllowedAreas(DockWidgetArea::OuterDockAreas);

    // create other dock widgets
//    QListView* lv = new QListView();
//    CDockWidget* lvDockWidget = new CDockWidget("Table 1");
//    lvDockWidget->setWidget(lv);
//    lvDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
//    lvDockWidget->setMinimumSize(200,150);
//    const auto autoHideContainer = dockManager->addAutoHideDockWidget(SideBarLocation::SideBarLeft, lvDockWidget);
////    autoHideContainer->setSize(480);
//    ui->menuView->addAction(lvDockWidget->toggleViewAction());

//    QTableWidget* propertiesTable = new QTableWidget();
//    propertiesTable->setColumnCount(3);
//    propertiesTable->setRowCount(10);
//    CDockWidget* PropertiesDockWidget = new CDockWidget("Properties");
//    PropertiesDockWidget->setWidget(propertiesTable);
//    PropertiesDockWidget->setMinimumSizeHintMode(CDockWidget::MinimumSizeHintFromDockWidget);
//    PropertiesDockWidget->resize(250, 150);
//    PropertiesDockWidget->setMinimumSize(200,150);
//    DockManager->addDockWidget(DockWidgetArea::RightDockWidgetArea, PropertiesDockWidget, CentralDockArea);
//    ui->menuView->addAction(PropertiesDockWidget->toggleViewAction());
}

FlowLink::~FlowLink()
{
    delete ui;
}

