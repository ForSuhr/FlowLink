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
    // language selection
    QLabel *languageLabel = new QLabel();
    languageLabel->setTextFormat(Qt::RichText);
    languageLabel->setText(tr("<b>Language</b>"));
    QComboBox *languageComboBox = new QComboBox();
    languageComboBox->addItem("English");
    languageComboBox->addItem("Deutsch");
    languageComboBox->addItem("中文");
    languageComboBox->setCurrentText(config.value("Appearance/Language").toString());
    QLabel *restartWarningLabel = new QLabel(tr("* The language will be changed after restarting the application."));
    // style selection
    QLabel *styleLabel = new QLabel();
    styleLabel->setTextFormat(Qt::RichText);
    styleLabel->setText(tr("<b>Style</b>"));
    QComboBox *styleComboBox = new QComboBox();
    styleComboBox->addItem("Lumos");
    languageComboBox->setCurrentText(config.value("Appearance/Style").toString());

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
            { 
                config.beginGroup("Appearance");
                config.setValue("Language", languageName);
                config.endGroup(); });
    // save the style setting to config file
    connect(styleComboBox, &QComboBox::currentTextChanged, [&](QString styleName)
            { 
                config.beginGroup("Appearance");
                config.setValue("Style", styleName);
                config.endGroup(); });
}

void PrefWindow::setupWindowProperties()
{
    setWindowTitle(tr("Preferences"));
    resize(500, 600);
}
