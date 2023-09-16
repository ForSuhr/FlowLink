#include "./ui_ChatWindow.h"
#include "ChatWindow.h"

ChatWindow::ChatWindow(Device device, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->lineEditMsg->setAlignment(Qt::AlignCenter);
    connect(ui->lineEditMsg, &QLineEdit::returnPressed, this, &ChatWindow::on_btnSendMsg_clicked); // hotkey binding

    m_device = device;
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setupMsgReceiveConnection()
{
    connect(m_tcpReceiver, &TcpReceiver::msgSignal, [&](QString msg)
            { leftAlignedAppend(this, msg); });
}

QString ChatWindow::msgText()
{
    return ui->lineEditMsg->text();
}

/// @brief send the message in the editor, after that, clear the message
void ChatWindow::on_btnSendMsg_clicked()
{
    QString msg = msgText();
    if (!msg.isEmpty())
    {
        m_tcpSender->sendMsg(msg);
        rightAlignedAppend(this, msg);
        ui->lineEditMsg->clear();
    }
}

/// @brief select a file to transfer
void ChatWindow::on_btnSendFile_clicked()
{
    PLOG_DEBUG << "Select a file to transfer.";

    QString filePath = QFileDialog::getOpenFileName(this, "Select File", "/", "All FIles(*.*)");
    if (!filePath.isEmpty())
    {
        PLOG_DEBUG << "Start tranferring.";
        m_tcpSender->sendBin(filePath);
    }
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
