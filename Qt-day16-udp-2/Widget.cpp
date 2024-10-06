#include "Widget.h"
#include "./ui_widget.h"
#include <QMessageBox>


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , MSocket(new QUdpSocket(this)) // 初始化列表
{
    ui->setupUi(this);
    ui->UnjoinBtn->setEnabled(false); // 关闭退出按钮,直到链接成功后

    // 连接信号与槽
    connect(MSocket, &QUdpSocket::readyRead, this, [this](){
        while (MSocket->hasPendingDatagrams())
        {
            QByteArray datagram;
            QHostAddress sender;
            quint16 senderPort;
            //添加发送时间信息
            QString time=QDateTime::currentDateTime().toString("yyyy-MM-dd HH::mm::ss dddd");
            datagram.resize(MSocket->pendingDatagramSize());

            MSocket->setObjectName("My Dear：");//设置发送人姓名
            QString SendName=MSocket->objectName();

            // 接收数据报
            MSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            // 显示接收到的消息
            QString message = QString::fromUtf8(datagram);
            //把信息加入到聊天框
            ui->SendText->append(time+'\n'+SendName+message);
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

// 加入组播
void Widget::on_JoinBtn_clicked()
{
    QString address = ui->AdressEdit->text();
    QHostAddress multicastAddress(address);
    //注意：UDP预留组播地址 239.0.0.0 -- 239.255.255.255
    if (!multicastAddress.isNull() && MSocket->joinMulticastGroup(multicastAddress))
    {
        QMessageBox::information(this, "提示", "成功加入组播: " + address);
        ui->JoinBtn->setEnabled(false);
        ui->UnjoinBtn->setEnabled(true); // 启用退出按钮
    }
    else
    {
        QMessageBox::warning(this, "警告", "加入失败");
    }
}

// 退出组播
void Widget::on_UnjoinBtn_clicked()
{
    QString address = ui->AdressEdit->text();
    QHostAddress multicastAddress(address);
    if (MSocket->leaveMulticastGroup(multicastAddress))
    {
        QMessageBox::information(this, "提示", "成功退出组播: " + address);
        ui->JoinBtn->setEnabled(true); // 启用加入按钮
        ui->UnjoinBtn->setEnabled(false); // 禁用退出按钮
        ui->SendBtn->setEnabled(false);//退出后禁用发送消息按钮
    }
    else
    {
        QMessageBox::warning(this, "警告", "退出失败");
    }
}

// 绑定端口号
void Widget::on_pushButton_clicked()
{
    int port = ui->PortEdit->text().toInt();
    if (port < 1 || port > 65535)
    {
        QMessageBox::warning(this, "警告", "端口号不合法，请输入1到65535之间的数字");
        return;
    }

    if (MSocket->bind(QHostAddress::AnyIPv4, port, QUdpSocket::ShareAddress))
    {
        QMessageBox::information(this, "提示", "成功绑定端口: " + QString::number(port));
        ui->pushButton->setEnabled(false);
    }
    else
    {
        QMessageBox::warning(this, "警告", "绑定端口失败，请检查端口是否被占用");
    }
}

// 发送消息
void Widget::on_SendBtn_clicked()
{
    QString message = ui->SendEdit->text(); // 假设有一个文本框用于输入消息
    QString address = ui->AdressEdit->text();
    int port = ui->PortEdit->text().toInt();
    QHostAddress multicastAddress(address);

    if (message.isEmpty())
    {
        QMessageBox::warning(this, "警告", "消息内容不能为空");
        return;
    }

    QByteArray data = message.toUtf8();
    //记录发送信息
    qint64 bytesSent = MSocket->writeDatagram(data, multicastAddress, port);

    if (bytesSent == -1)
    {
        QMessageBox::warning(this, "警告", "发送失败: " + MSocket->errorString());
    }
    else
    {
        QMessageBox::information(this, "提示", "消息已发送");
        ui->SendEdit->clear(); // 清空输入框
    }
}
