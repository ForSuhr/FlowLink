#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QWidget>

#include "../utils/TabBar.h"

class PrefWindow : public TabWidget
{

    Q_OBJECT

public:
    explicit PrefWindow(QWidget *parent = nullptr);
};

#endif