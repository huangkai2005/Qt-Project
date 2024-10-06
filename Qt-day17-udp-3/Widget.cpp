#include "Widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
    , MSocket(new QUdpSocket(this))//初始化列表
    ,port(0)
{
    ui->setupUi(this);
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

void Widget::on_SendBtn_clicked()
{
    auto text=ui->SendEdit->text();//获取发送信息

    //发送数据报
    MSocket->writeDatagram(text.toUtf8(),QHostAddress::Broadcast,port);
    ui->SendEdit->clear();
}
// 绑定端口号
void Widget::on_pushButton_clicked()
{
    port = ui->PortEdit->text().toInt();
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
