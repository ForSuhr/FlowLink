#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>

#include "../utils/TabBar.h"
#include "../utils/ConfigParser.h"

class PrefWindow : public TabWidget
{
    Q_OBJECT

public:
    explicit PrefWindow(QWidget *parent = nullptr);

private:
    void setupDefaultPreferences();
    void setupCommonTab();
    void setupApperanceTab();
    void setupWindowProperties();

    /* tabs */
    QWidget *m_commonTab = nullptr;
    QWidget *m_apperanceTab = nullptr;

    /* members */
    QLineEdit *downloadDirectoryLineEdit = nullptr;
};

#endif