#include "ProgressWindow.h"

ProgressWindow::ProgressWindow(QWidget *parent) : QWidget(parent),
                                                  m_progressWidgetMap(new std::unordered_map<QString, QWidget *>)
{
    setupUI();
}

ProgressWindow::~ProgressWindow()
{
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

void ProgressWindow::createProgressWidget(const QString &filename, qint64 totalFileBytes)
{
    // widgets
    QWidget *progressWidget = new QWidget();
    progressWidget->setObjectName("progressWidget");
    progressWidget->setFixedHeight(64);
    QLabel *filenameLabel = new QLabel(filename);
    ConvertedNumber fileTotalSize = BytesConvert(totalFileBytes);
    QLabel *fileCurrentSizeLabel = new QLabel("0GB");
    fileCurrentSizeLabel->setObjectName("fileCurrentSizeLabel");
    QLabel *fileTotalSizeLabel = new QLabel(QString("/%1").arg(QString::number(fileTotalSize.number, 'f', 2) + fileTotalSize.unit));
    QProgressBar *progressBar = new QProgressBar();
    progressBar->setObjectName("progressBar");
    progressBar->setFixedHeight(5);
    progressBar->setTextVisible(false);
    progressBar->setMaximum(totalFileBytes);

    // layout
    QVBoxLayout *vbox = new QVBoxLayout(progressWidget);
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->setSpacing(0);
    hbox->setContentsMargins(0, 0, 0, 0);

    // add widgets to layout
    m_vbox->insertWidget(m_vbox->count() - 1, progressWidget);
    vbox->addWidget(filenameLabel);
    hbox->addWidget(fileCurrentSizeLabel);
    hbox->addWidget(fileTotalSizeLabel);
    hbox->addStretch();
    vbox->addLayout(hbox);
    vbox->addWidget(progressBar);

    // store the progress widget to a map for the convenience we query it
    if (m_progressWidgetMap->find(filename) == m_progressWidgetMap->end())
        (*m_progressWidgetMap)[filename] = progressWidget;
}

void ProgressWindow::updateProgress(const QString &filename, qint64 receivedBytes, qint64 totalBytes)
{
    // retrieve the progress widget from the map by filename
    QWidget *progressWidget = (*m_progressWidgetMap)[filename];

    // update the progress in number
    QLabel *fileCurrentSizeLabel = progressWidget->findChild<QLabel *>("fileCurrentSizeLabel");
    ConvertedNumber fileCurrentSize = BytesConvert(receivedBytes);
    fileCurrentSizeLabel->setText(QString::number(fileCurrentSize.number, 'f', 2) + fileCurrentSize.unit);

    // update the progress in progress bar
    QProgressBar *progressBar = progressWidget->findChild<QProgressBar *>("progressBar");
    progressBar->setValue(receivedBytes);
}
