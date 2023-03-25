#ifndef FLOWLINK_H
#define FLOWLINK_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class FlowLink; }
QT_END_NAMESPACE

class FlowLink : public QMainWindow
{
    Q_OBJECT

public:
    FlowLink(QWidget *parent = nullptr);
    ~FlowLink();

private:
    Ui::FlowLink *ui;
};
#endif // FLOWLINK_H
