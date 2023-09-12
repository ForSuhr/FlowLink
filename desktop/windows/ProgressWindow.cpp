#include "ProgressWindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QWidget(parent)
{
    setupUI();
    createProgressWidget();
    createProgressWidget();
    createProgressWidget();
    createProgressWidget();
}

void ProgressWindow::setupUI()
{
    m_vbox = new QVBoxLayout(this);
    m_vbox->addStretch();
}

QVBoxLayout *ProgressWindow::layout()
{
    return m_vbox;
}

void ProgressWindow::createProgressWidget()
{
    // widgets
    QWidget *progressWidget = new QWidget(this);
    progressWidget->setObjectName("progressWidget");
    progressWidget->setFixedHeight(64);
    QLabel *fileName = new QLabel("file name");
    QLabel *progressInNumber = new QLabel("progressInNumber");
    QProgressBar *progressBar = new QProgressBar();

    // layout
    QVBoxLayout *vbox = new QVBoxLayout(progressWidget);

    // add widgets to layout
    m_vbox->insertWidget(m_vbox->count() - 1, progressWidget);
    vbox->addWidget(fileName);
    vbox->addWidget(progressInNumber);
    vbox->addWidget(progressBar);

    progressBar->setValue(50);
    progressBar->setFixedHeight(5);
    progressBar->setTextVisible(false);
}
