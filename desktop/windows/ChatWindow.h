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
    explicit ChatWindow(QString address, int port, QWidget *parent = nullptr);
    ~ChatWindow();

    QString msgText();

    /* friend functions */
    friend void leftAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void rightAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void centerAlignedAppend(const ChatWindow *chatWindow, const QString &text);

    /* slots */
private slots:
    void on_btnSelectFile_clicked();
    void on_btnSend_clicked();

private:
    Ui::ChatWindow *ui;

    /* network */
    TcpReceiver *m_tcpReceiver = nullptr;
    TcpSender *m_tcpSender = nullptr;
};

#endif
