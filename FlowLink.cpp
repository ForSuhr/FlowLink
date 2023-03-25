#include "FlowLink.h"
#include "./ui_FlowLink.h"

FlowLink::FlowLink(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FlowLink)
{
    ui->setupUi(this);
}

FlowLink::~FlowLink()
{
    delete ui;
}

