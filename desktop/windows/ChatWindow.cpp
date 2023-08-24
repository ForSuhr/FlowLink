#include "./ui_ChatWindow.h"
#include "ChatWindow.h"

ChatWindow::ChatWindow(QString address, int port, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ChatWindow),
      m_tcpReceiver(new TcpReceiver(port))
{
    ui->setupUi(this);

    // receive message connection
    connect(m_tcpReceiver, &TcpReceiver::msgSignal, [&](const QString &msg)
            { leftAlignedAppend(this, msg); });

    // connect to the device
    m_tcpSender = new TcpSender(address);
    centerAlignedAppend(this, address + " connected");
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

QString ChatWindow::msgText()
{
    return ui->textEditMsg->toPlainText();
}

/// @brief send the message in the editor, after that, clear the message
void ChatWindow::on_btnSend_clicked()
{
    QString msg = msgText();
    m_tcpSender->sendMsg(msg);
    rightAlignedAppend(this, msg);
    ui->textEditMsg->clear();
}

/// @brief select a file to transfer
void ChatWindow::on_btnSelectFile_clicked()
{
    PLOG_DEBUG << "Select a file to transfer.";

    QString fileName = QFileDialog::getOpenFileName(this, "Select File", "/", "All FIles(*.*)");
    if (!fileName.isEmpty())
    {
        PLOG_DEBUG << "Start tranferring.";
        m_tcpSender->sendBin(fileName);
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
