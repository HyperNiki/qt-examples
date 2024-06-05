#include "clientwindow.h"
#include "ui_clientwindow.h"

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ClientWindow)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &ClientWindow::deleteLater);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}


void ClientWindow::on_pushButton_clicked()
{
    socket->connectToHost("127.0.0.1", 2323);
}

void ClientWindow::SendToServer(QString str)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_6_2);
    out << str;
    socket->write(Data);
}

void ClientWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status()==QDataStream::Ok)
    {
        QString str;
        in >> str;
        ui->textBrowser->append(str);
    }
    else
    {
        ui->textBrowser->append("Error: incorrect message");
    }
}


void ClientWindow::on_pushButton_2_clicked()
{
    SendToServer(ui->lineEdit->text());
}


void ClientWindow::on_lineEdit_returnPressed()
{
    SendToServer(ui->lineEdit->text());
}

