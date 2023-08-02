#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QWidget>
#include <Qt>

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

    QString msgText();

    friend void leftAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void rightAlignedAppend(const ChatWindow *chatWindow, const QString &text);
    friend void centerAlignedAppend(const ChatWindow *chatWindow, const QString &text);

signals:
    void onBtnSendClickedSignal();

private:
    Ui::ChatWindow *ui;

private slots:
    void onBtnSendClicked();
};

#endif
