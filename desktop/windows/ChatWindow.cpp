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

void leftAlignedAppend(const ChatWindow *chatWindow, const QString &text)
{
    QTextCursor textCursor = chatWindow->ui->textEditView->textCursor();
    QTextCursor leftAlignedCursor = QTextCursor(textCursor);
    QTextBlockFormat textBlockFormatLeft = QTextBlockFormat();
    textBlockFormatLeft.setAlignment(Qt::AlignLeft);
    leftAlignedCursor.insertBlock(textBlockFormatLeft);
    leftAlignedCursor.movePosition(QTextCursor::EndOfLine);
    leftAlignedCursor.insertText("<Guest> " + text);
    chatWindow->ui->textEditView->ensureCursorVisible();
}

void rightAlignedAppend(const ChatWindow *chatWindow, const QString &text)
{
    QTextCursor textCursor = chatWindow->ui->textEditView->textCursor();
    QTextCursor rightAlignedCursor = QTextCursor(textCursor);
    QTextBlockFormat textBlockFormatRight = QTextBlockFormat();
    textBlockFormatRight.setAlignment(Qt::AlignRight);
    rightAlignedCursor.insertBlock(textBlockFormatRight);
    rightAlignedCursor.movePosition(QTextCursor::EndOfLine);
    rightAlignedCursor.insertText("<Host> " + text);
    chatWindow->ui->textEditView->ensureCursorVisible();
}

void centerAlignedAppend(const ChatWindow *chatWindow, const QString &text)
{
    QTextCursor textCursor = chatWindow->ui->textEditView->textCursor();
    QTextCursor centerAlignedCursor = QTextCursor(textCursor);
    QTextBlockFormat textBlockFormatCenter = QTextBlockFormat();
    textBlockFormatCenter.setAlignment(Qt::AlignCenter);
    centerAlignedCursor.insertBlock(textBlockFormatCenter);
    centerAlignedCursor.movePosition(QTextCursor::EndOfLine);
    centerAlignedCursor.insertText("<System> " + text);
    chatWindow->ui->textEditView->ensureCursorVisible();
}
