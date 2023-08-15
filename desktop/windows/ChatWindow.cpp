#include "./ui_ChatWindow.h"
#include "ChatWindow.h"

ChatWindow::ChatWindow(QString address, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ChatWindow),
      m_tcpReceiver(new TcpReceiver),
      m_tcpSender(new TcpSender(address))
{
    ui->setupUi(this);

    // device connected
    centerAlignedAppend(this, address + " connected");

    // send message
    connect(ui->btnSend, &QPushButton::clicked, [&]()
            { QString msg = msgText();
            m_tcpSender->sendMsg(msg);
            rightAlignedAppend(this, msg); });

    // receive message
    connect(m_tcpReceiver, &TcpReceiver::msgSignal, [&](const QString &msg)
            { leftAlignedAppend(this, msg); });
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

QString ChatWindow::msgText()
{
    return ui->textEditMsg->toPlainText();
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
