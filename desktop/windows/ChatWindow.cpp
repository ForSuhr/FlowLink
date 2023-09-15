#include "./ui_ChatWindow.h"
#include "ChatWindow.h"

ChatWindow::ChatWindow(QString address, int port, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::ChatWindow)
{
    ui->setupUi(this);
    ui->lineEditMsg->setAlignment(Qt::AlignCenter);

    // get port
    m_port = port;
    PLOG_DEBUG << "port for current chat window: " << m_port << " for msg and " << m_port + 1 << " for bin";

    // initialize tcp receiver, i.e. set up a tcp server to listen to the given port
    m_tcpReceiver = new TcpReceiver(m_port);

    // receive message connection
    connect(m_tcpReceiver, &TcpReceiver::msgSignal, [&](const QString &msg)
            { leftAlignedAppend(this, msg); });

    // shortcut connection
    connect(ui->lineEditMsg, &QLineEdit::returnPressed, this, &ChatWindow::on_btnSendMsg_clicked);

    // connect to the device
    m_tcpSender = new TcpSender(address, m_port);
    centerAlignedAppend(this, address + " connected");
}

ChatWindow::~ChatWindow()
{
    delete ui;
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
