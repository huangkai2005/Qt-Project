#include "Widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    MySocket = new QTcpSocket(this); // 确保 MySocket 是这个 Widget 的子对象

    // 连接信号与槽
    connect(MySocket, &QTcpSocket::readyRead, this, [this]() {
        QByteArray data = MySocket->readAll(); // 读取所有数据
        qDebug() << "Received data:" << data; // 调试信息
        ui->GetText->append(QString(data)); // 更新 UI 中的文本框
    });

    connect(MySocket, &QTcpSocket::disconnected, this, [this]() {
        QMessageBox::information(this, "提示", "连接已断开"); // 提示用户
        MySocket->deleteLater(); // 清理已断开的套接字
        MySocket = nullptr; // 将指针重置为 nullptr
        ui->SocketBtn->setEnabled(true); // 重新启用连接按钮
    });

    connect(MySocket, &QTcpSocket::errorOccurred, this, [this](QAbstractSocket::SocketError socketError) {
        qDebug() << "Socket error:" << socketError << MySocket->errorString(); // 打印错误信息
        QMessageBox::warning(this, "连接错误", MySocket->errorString());
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_SocketBtn_clicked()
{
    if (ui->IPEdit->text().isEmpty() || ui->PortEdit->text().isEmpty()) {
        QMessageBox::warning(this, "提示", "IP 地址和端口不能为空！");
        return;
    }

    const QString ip = ui->IPEdit->text();
    bool ok;
    int port = ui->PortEdit->text().toInt(&ok);

    if (!ok || port <= 0 || port > 65535) {
        QMessageBox::warning(this, "提示", "请输入有效的端口号！");
        return;
    }

    if (MySocket && MySocket->state() == QAbstractSocket::ConnectedState) {
        QMessageBox::warning(this, "提示", "已经连接到服务器！");
        return;
    }
    // 禁用代理
    MySocket->setProxy(QNetworkProxy::NoProxy);
    qDebug() << "Connecting to:" << ip << "on port:" << port; // 调试信息
    MySocket->connectToHost(QHostAddress(ip), port); // 连接到主机

    connect(MySocket, &QTcpSocket::connected, this, [this]() {
        QMessageBox::information(this, "提示", "连接成功！");
        ui->SocketBtn->setEnabled(false); // 禁用按钮
    });
}

void Widget::on_SendBtn_clicked()
{
    if (MySocket && MySocket->state() == QAbstractSocket::ConnectedState) { // 确保有有效的客户端连接
        QByteArray message = ui->SendEdit->text().toUtf8(); // 发送消息
        MySocket->write(message); // 发送消息
        ui->SendEdit->clear(); // 清空输入框
    } else {
        QMessageBox::warning(this, "提示", "没有连接的客户端！");
    }
}
