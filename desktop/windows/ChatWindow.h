#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

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

    friend void appendTextToChatWindow(const ChatWindow *chatWindow, const QString &text);

signals:
    void onBtnSendClickedSignal();

private:
    Ui::ChatWindow *ui;

private slots:
    void onBtnSendClicked();
};

#endif
