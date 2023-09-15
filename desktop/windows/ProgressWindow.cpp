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
    if (m_progressWidgetMap->find(filename) != m_progressWidgetMap->end())
    {
        (*m_progressWidgetMap)[filename]->deleteLater();
        m_progressWidgetMap->erase(filename);
    }

    // widgets
    // row 1
    QWidget *progressWidget = new QWidget();
    progressWidget->setObjectName("progressWidget");
    progressWidget->setFixedHeight(64);
    QLabel *filenameLabel = new QLabel(filename);
    filenameLabel->setObjectName("filenameLabel");
    QPushButton *btnClose = new QPushButton(progressWidget);
    btnClose->setObjectName("btnClose");
    btnClose->setFixedSize(12, 16);
    // row 2
    ConvertedNumber fileTotalSize = BytesConvert(totalFileBytes);
    QLabel *fileCurrentSizeLabel = new QLabel("0GB");
    fileCurrentSizeLabel->setObjectName("fileCurrentSizeLabel");
    QLabel *fileTotalSizeLabel = new QLabel(QString("/%1").arg(QString::number(fileTotalSize.number, 'f', 2) + fileTotalSize.unit));
    QLabel *finishSign = new QLabel();
    finishSign->setObjectName("finishSign");
    QPixmap pixmap = QPixmap(R"(:/asset/style/lumos/finishSign.png)");
    finishSign->setPixmap(pixmap);
    finishSign->setFixedSize(pixmap.size());
    finishSign->setVisible(false);
    QPushButton *btnFolderOpen = new QPushButton(progressWidget);
    btnFolderOpen->setObjectName("btnFolderOpen");
    btnFolderOpen->setFixedSize(16, 16);
    btnFolderOpen->setVisible(false);
    // row 3
    QProgressBar *progressBar = new QProgressBar();
    progressBar->setObjectName("progressBar");
    progressBar->setFixedHeight(5);
    progressBar->setTextVisible(false);
    progressBar->setMaximum(totalFileBytes);

    // layout
    QVBoxLayout *vbox = new QVBoxLayout(progressWidget);
    QHBoxLayout *hbox1 = new QHBoxLayout();
    QHBoxLayout *hbox2 = new QHBoxLayout();
    hbox2->setSpacing(0);
    hbox2->setContentsMargins(0, 0, 0, 0);

    // add widgets to layout
    m_vbox->insertWidget(m_vbox->count() - 1, progressWidget);
    hbox1->addWidget(filenameLabel);
    hbox1->addStretch();
    hbox1->addWidget(btnClose);
    hbox2->addWidget(fileCurrentSizeLabel);
    hbox2->addWidget(fileTotalSizeLabel);
    hbox2->addSpacing(5);
    hbox2->addWidget(finishSign);
    hbox2->addSpacing(5);
    hbox2->addWidget(btnFolderOpen);
    hbox2->addStretch();
    vbox->addLayout(hbox1);
    vbox->addLayout(hbox2);
    vbox->addWidget(progressBar);

    // connection
    connect(btnClose, &QPushButton::clicked, this, &ProgressWindow::deleteProgressWidget);
    connect(btnFolderOpen, &QPushButton::clicked, [&]()
            { QDesktopServices::openUrl(QUrl(config.value("common/downloadDirectory").toString())); });

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
    if (progressBar->value() == progressBar->maximum())
    {
        QLabel *finishSign = progressWidget->findChild<QLabel *>("finishSign");
        finishSign->setVisible(true);
        QPushButton *btnFolderOpen = progressWidget->findChild<QPushButton *>("btnFolderOpen");
        btnFolderOpen->setVisible(true);
    }
}

void ProgressWindow::deleteProgressWidget()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    button->parentWidget()->deleteLater();
    QString filename = button->parentWidget()->findChild<QLabel *>("filenameLabel")->text();
    m_progressWidgetMap->erase(filename);
}
