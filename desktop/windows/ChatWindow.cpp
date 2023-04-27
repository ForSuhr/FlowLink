#include "./ui_ChatWindow.h"
#include "ChatWindow.h"

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    connect(ui->btnSend, &QPushButton::clicked, this, &ChatWindow::onBtnSendClicked);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

QString ChatWindow::msgText()
{
    return ui->textEditMsg->toPlainText();
}

void ChatWindow::onBtnSendClicked()
{
    emit onBtnSendClickedSignal();
}

void appendTextToChatWindow(const ChatWindow *chatWindow, const QString &text)
{
    chatWindow->ui->textEditView->append(text);
}
