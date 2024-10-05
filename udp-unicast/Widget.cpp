#include "Widget.h"
#include "./ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    MSocket = new QUdpSocket(this);

    // 连接信号与槽，处理接收到的数据
    connect(MSocket, &QUdpSocket::readyRead, this, [this]() {
        while (MSocket->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(MSocket->pendingDatagramSize());
            QHostAddress sender;
            quint16 senderPort;

            // 读取数据报
            MSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

            // 更新 UI 中的文本框
            ui->GetText->append(QString(datagram));
        }
    });
}

Widget::~Widget()
{
    delete ui;
}

// 绑定端口号
void Widget::on_SendBtn_clicked()
{
    // 获取要发送的文本
    QString message = ui->SendEdit->text(); // 假设有一个输入框用于输入文本
    QString ip = ui->IpEdit->text();

    if (message.isEmpty()) {
        return; // 如果没有输入，不发送
    }

    int targetPort = ui->PortEdit->text().toInt(); // 根据需要设置目标端口
    // 定义目标IP和端口号
    QHostAddress targetAddress = QHostAddress(ip); // 使用指定的地址

    // 发送数据
    MSocket->writeDatagram(message.toUtf8(), targetAddress, targetPort);

    // 清空输入框
    ui->SendEdit->clear();
}

void Widget::on_btn_clicked()
{
    int targetPort = ui->PortEdit->text().toInt(); // 根据需要设置目标端口
    // ShareAddress 共享地址 可以接收到自己的信号
    bool ok = MSocket->bind(targetPort, QAbstractSocket::ShareAddress);

    if (ok) {
        QMessageBox::information(this, "提示", "绑定端口成功");
        ui->SendBtn->setEnabled(true); // 启用发送按钮
        ui->btn->setEnabled(false); // 禁用绑定按钮
    } else {
        QMessageBox::warning(this, "警告", "绑定端口失败");
    }
}
