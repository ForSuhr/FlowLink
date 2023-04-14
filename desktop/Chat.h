#ifndef CHAT_H
#define CHAT_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class ChatWindow;
}
QT_END_NAMESPACE

class ChatWindow : public QWidget
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = nullptr);
    ~ChatWindow();

private:
    Ui::ChatWindow *ui;
};

#endif // CHAT_H
