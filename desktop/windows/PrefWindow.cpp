#include "PrefWindow.h"

PrefWindow::PrefWindow(QWidget *parent) : TabWidget(parent)
{
    setupApperanceTab();
    setupWindowProperties();
}

void PrefWindow::setupApperanceTab()
{
    /* add new tab */
    m_apperanceTab = new QWidget(this);
    addTab(m_apperanceTab, tr("Appearance"));

    /* widget */
    QLabel *languageLabel = new QLabel(tr("Language"));
    QComboBox *languageComboBox = new QComboBox();
    languageComboBox->addItem("English");
    languageComboBox->addItem("Deutsch");
    languageComboBox->addItem("中文");
    languageComboBox->setCurrentText(config.value("Appearance/Language").toString());
    QLabel *restartWarningLabel = new QLabel(tr("* The language will be changed after restarting the application."));

    /* layout */
    QVBoxLayout *vbox = new QVBoxLayout();
    m_apperanceTab->setLayout(vbox);

    /* add widgets to layout */
    vbox->addWidget(languageLabel);
    vbox->addWidget(languageComboBox);
    vbox->addWidget(restartWarningLabel);
    vbox->addStretch();

    /* signal-slot */
    // save the language setting to config file
    connect(languageComboBox, &QComboBox::currentTextChanged, [&](QString languageName)
            { 
                config.beginGroup("Appearance");
                config.setValue("Language", languageName);
                config.endGroup(); });
}

void PrefWindow::setupWindowProperties()
{
    setWindowTitle(tr("Preferences"));
    resize(500, 600);
}
