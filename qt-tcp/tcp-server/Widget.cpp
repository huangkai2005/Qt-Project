#include "Widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , MyServer(new QTcpServer(this)) // 初始化 MyServer
    , MySocket(nullptr) // 初始化 MySocket
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_HostBtn_clicked()
{
    // 点击获取主机信息
    QString hostname = QHostInfo::localHostName(); // 获取本机名字
    QHostInfo info = QHostInfo::fromName(hostname); // 通过主机名字获取主机信息对象
    QVector<QHostAddress> addresses = info.addresses(); // 获取主机网络IP地址

    ui->Edit->clear(); // 清空之前内容
    ui->Edit->append("Hostname: " + hostname);

    for (const auto &address : addresses) // 遍历容器
    {
        ui->Edit->append("*************");
        ui->Edit->append(address.toString());
    }
}

void Widget::on_MBtn_clicked() // monitor-button 监听
{
    if (ui->PortEdit->text().isEmpty())
        return;

    MPort = ui->PortEdit->text().toInt(); // 获取填写的端口号
    if (MPort < 1 || MPort > 65535)
    {
        QMessageBox::warning(this, "提示", "端口号无效！");
        return;
    }

    if (MyServer->listen(QHostAddress::Any, MPort)) // 服务器监听
    {
        // 监听成功
        QMessageBox::information(this, "提示", "监听成功！");

        // 当有客户端成功连接到服务器时，会触发 newConnection 信号
        connect(MyServer, &QTcpServer::newConnection, this, [this]() {
            MySocket = MyServer->nextPendingConnection(); // 获取与新客户端的连接

            // 连接信号与槽，处理接收到的数据
            connect(MySocket, &QTcpSocket::readyRead, this, [this]() {
                QByteArray data = MySocket->readAll(); // 读取所有数据
                ui->GetText->append(QString(data)); // 更新 UI 中的文本框
            });

            connect(MySocket, &QTcpSocket::disconnected, this, [this]() {
                MySocket->deleteLater(); // 清理已断开的套接字
                MySocket = nullptr; // 将指针重置为 nullptr
            });
        });
    }
    else
    {
        // 监听失败
        QMessageBox::warning(this, "提示", "监听失败！");
    }
}

void Widget::on_SendBtn_clicked()
{
    // 点击按钮发送消息
    if (MySocket) // 确保有有效的客户端连接
    {
        MySocket->write(ui->SendEdit->text().toUtf8()); // 发送消息
        ui->SendEdit->clear(); // 清空输入框
    }
    else
    {
        QMessageBox::warning(this, "提示", "没有连接的客户端！");
    }
}
