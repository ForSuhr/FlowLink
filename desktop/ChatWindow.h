#ifndef CHATWINDOW_H
#define CHATWINDOW_H

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

    friend void setTextEditView(const ChatWindow *chatWindow, const QString &text);

private:
    Ui::ChatWindow *ui;
};

#endif
