#include "ProgressWindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QWidget(parent)
{
    setupUI();
    createProgressWidget("a.out", 10LL);
    createProgressWidget("b.out", 3000 * 1024LL);
    createProgressWidget("c.out", 2000LL);
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

void ProgressWindow::createProgressWidget(const QString &filename, qint64 fileTotalBytes)
{
    // widgets
    QWidget *progressWidget = new QWidget(this);
    progressWidget->setObjectName("progressWidget");
    progressWidget->setFixedHeight(64);
    QLabel *fileName = new QLabel(filename);
    ConvertedNumber fileTotalSize = BytesConvert(fileTotalBytes);
    fileCurrentSizeLabel = new QLabel("0GB");
    QLabel *fileTotalSizeLabel = new QLabel(QString("/%1").arg(QString::number(fileTotalSize.number, 'f', 2) + fileTotalSize.unit));
    progressBar = new QProgressBar();

    // layout
    QVBoxLayout *vbox = new QVBoxLayout(progressWidget);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(0);
    hbox->setContentsMargins(0, 0, 0, 0);

    // add widgets to layout
    m_vbox->insertWidget(m_vbox->count() - 1, progressWidget);
    vbox->addWidget(fileName);
    hbox->addWidget(fileCurrentSizeLabel);
    hbox->addWidget(fileTotalSizeLabel);
    hbox->addStretch();
    vbox->addLayout(hbox);
    vbox->addWidget(progressBar);

    progressBar->setValue(50);
    progressBar->setFixedHeight(5);
    progressBar->setTextVisible(false);
}
