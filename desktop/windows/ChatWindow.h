#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QWidget>
#include <Qt>

#include "../network/TcpSender.h"
#include "../network/TcpReceiver.h"

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
    explicit ChatWindow(QString address, QWidget *parent = nullptr);
    ~ChatWindow();

    QString msgText();

    friend void leftAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void rightAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void centerAlignedAppend(const ChatWindow *chatWindow, const QString &text);

private:
    Ui::ChatWindow *ui;

    /* network */
    TcpReceiver *m_tcpReceiver = nullptr;
    TcpSender *m_tcpSender = nullptr;
};

#endif
