#include "PrefWindow.h"

PrefWindow::PrefWindow(QWidget *parent) : TabWidget(parent)
{
    setupDefaultPreferences();
    setupCommonTab();
    setupApperanceTab();
    setupWindowProperties();
}

/// @brief create a default preferences file if it does not exists
void PrefWindow::setupDefaultPreferences()
{
    if (!QFile::exists(CONFIG_PATH))
    {
        // group "Common"
        config.beginGroup("common");
        config.setValue("showLocalHost", false);
        config.setValue("downloadDirectory", QStandardPaths::writableLocation(QStandardPaths::DownloadLocation));
        config.endGroup();

        // group "Appearance"
        config.beginGroup("appearance");
        config.setValue("language", "English");
        config.setValue("style", "Lumos");
        config.endGroup();
    }
}

void PrefWindow::setupCommonTab()
{
    /* add new tab */
    m_commonTab = new QWidget(this);
    addTab(m_commonTab, tr("Common"));

    /* widget */
    // download directory selection
    QLabel *downloadDirectoryLabel = new QLabel();
    downloadDirectoryLabel->setTextFormat(Qt::RichText);
    downloadDirectoryLabel->setText(tr("<b>Download Directory</b>"));
    downloadDirectoryLineEdit = new QLineEdit();
    downloadDirectoryLineEdit->setReadOnly(true);
    downloadDirectoryLineEdit->setText(config.value("common/downloadDirectory").toString());
    QPushButton *selectDirectoryBtn = new QPushButton("...");

    /* layout */
    QVBoxLayout *vbox = new QVBoxLayout();
    m_commonTab->setLayout(vbox);
    QHBoxLayout *hbox = new QHBoxLayout();

    /* add widgets to layout */
    vbox->addWidget(downloadDirectoryLabel);
    hbox->addWidget(downloadDirectoryLineEdit);
    hbox->addWidget(selectDirectoryBtn);
    vbox->addLayout(hbox);
    vbox->addStretch();

    /* signal-slot */
    // save the language setting to config file
    connect(selectDirectoryBtn, &QPushButton::clicked, [&]()
            {
                QString downloadDirectory = QFileDialog::getExistingDirectory(this, tr("Select Directory"), config.value("common/downloadDirectory").toString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
                if (!downloadDirectory.isEmpty())
                {
                    config.setValue("common/downloadDirectory", downloadDirectory);
                    downloadDirectoryLineEdit->setText(downloadDirectory);
                } });
}

void PrefWindow::setupApperanceTab()
{
    /* add new tab */
    m_apperanceTab = new QWidget(this);
    addTab(m_apperanceTab, tr("Appearance"));

    /* widget */
    // language selection
    QLabel *languageLabel = new QLabel();
    languageLabel->setTextFormat(Qt::RichText);
    languageLabel->setText(tr("<b>Language</b>"));
    QComboBox *languageComboBox = new QComboBox();
    languageComboBox->addItem("English");
    languageComboBox->addItem("Deutsch");
    languageComboBox->addItem("中文");
    languageComboBox->setCurrentText(config.value("appearance/language").toString());
    QLabel *restartWarningLabel = new QLabel(tr("* The language will be changed after restarting the application."));
    // style selection
    QLabel *styleLabel = new QLabel();
    styleLabel->setTextFormat(Qt::RichText);
    styleLabel->setText(tr("<b>Style</b>"));
    QComboBox *styleComboBox = new QComboBox();
    styleComboBox->addItem("Lumos");
    languageComboBox->setCurrentText(config.value("appearance/style").toString());

    /* layout */
    QVBoxLayout *vbox = new QVBoxLayout();
    m_apperanceTab->setLayout(vbox);

    /* add widgets to layout */
    vbox->addWidget(languageLabel);
    vbox->addWidget(languageComboBox);
    vbox->addWidget(restartWarningLabel);
    vbox->addSpacing(20);
    vbox->addWidget(styleLabel);
    vbox->addWidget(styleComboBox);
    vbox->addStretch();

    /* signal-slot */
    // save the language setting to config file
    connect(languageComboBox, &QComboBox::currentTextChanged, [&](QString languageName)
            { config.setValue("appearance/language", languageName); });
    // save the style setting to config file
    connect(styleComboBox, &QComboBox::currentTextChanged, [&](QString styleName)
            { config.setValue("appearance/style", styleName); });
}

void PrefWindow::setupWindowProperties()
{
    setWindowTitle(tr("Preferences"));
    resize(500, 600);
}
