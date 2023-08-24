#ifndef TABBAR_H
#define TABBAR_H

#include <QTabBar>
#include <QTabWidget>
#include <QStyleOptionTab>
#include <QStylePainter>

class TabBar : public QTabBar
{
    Q_OBJECT

public:
    QSize tabSizeHint(int index) const;

protected:
    void paintEvent(QPaintEvent *event);
};

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit TabWidget(QWidget *parent = nullptr);
};

#endif