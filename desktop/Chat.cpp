#include "./ui_Chat.h"
#include "Chat.h"

ChatWindow::ChatWindow(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void setTextEditView(const ChatWindow *chatWindow, const QString &text)
{
    chatWindow->ui->textEditView->setText(text);
}
