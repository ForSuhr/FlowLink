#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <Qt>
#include <QWidget>
#include <QFileDialog>

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
    explicit ChatWindow(Device device, QWidget *parent = nullptr);
    ~ChatWindow();

    void setUpTcpReceiver(int port);
    void setUpTcpSender(QString address, int port);
    void setupMsgReceiveConnection();
    QString msgText();

    /* friend functions */
    friend void leftAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void rightAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void centerAlignedAppend(const ChatWindow *chatWindow, const QString &text);

    /* network */
    TcpReceiver *m_tcpReceiver = nullptr;
    TcpSender *m_tcpSender = nullptr;

    /*private slots */
private slots:
    void on_btnSendFile_clicked();
    void on_btnSendMsg_clicked();

    /*private field*/
private:
    Ui::ChatWindow *ui;
};

#endif
