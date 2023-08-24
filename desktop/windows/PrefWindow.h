#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QComboBox>

#include "../utils/TabBar.h"
#include "../utils/ConfigParser.h"

class PrefWindow : public TabWidget
{
    Q_OBJECT

public:
    explicit PrefWindow(QWidget *parent = nullptr);

private:
    void setupApperanceTab();
    void setupWindowProperties();

    QWidget *m_apperanceTab = nullptr;
};

#endif