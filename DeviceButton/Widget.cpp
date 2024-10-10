#include "Widget.h"
#include "./ui_widget.h"
#include <QVector>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initForm();
}

Widget::~Widget()
{
    delete ui;
}

//初始化地图
void Widget::initForm(){
    //设置背景地图
    ui->labMap->setStyleSheet("border-image:url(:/image/bg_call.jpg);");
    btn1 = new DeviceButton(ui->labMap);
    btn1->setText("#1");
    btn1->setGeometry(5, 5, 35, 35);

    btn2 = new DeviceButton(ui->labMap);
    btn2->setText("#2");
    btn2->setGeometry(45, 5, 35, 35);

    btn3 = new DeviceButton(ui->labMap);
    btn3->setText("#3");
    btn3->setGeometry(85, 5, 35, 35);

    btnStyle << ui->btnCircle << ui->btnPolice << ui->btnBubble << ui->btnBubble2 << ui->btnMsg << ui->btnMsg2;
    for(const auto &btn : btnStyle) {
        connect(btn, &QPushButton::clicked, this, &Widget::changeStyle);
    }

    btnColor << ui->btnGreen << ui->btnBlue << ui->btnRed << ui->btnGray << ui->btnBlack << ui->btnPurple << ui->btnYellow;
    for(const auto &btn : btnColor) {
        connect(btn, &QPushButton::clicked, this, &Widget::changeColor);
    }
}

//改变按钮形态槽函数
void Widget::changeStyle()
{
    QPushButton *btn = static_cast<QPushButton *>(sender());
    int index = btnStyle.indexOf(btn);
    DeviceButton::ButtonStyle style = static_cast<DeviceButton::ButtonStyle>(index);
    btn1->setButtonStyle(style);
    btn2->setButtonStyle(style);
    btn3->setButtonStyle(style);
}

//改变按钮颜色槽函数
void Widget::changeColor()
{
    QPushButton *btn = static_cast<QPushButton *>(sender());
    int index = btnColor.indexOf(btn);
    DeviceButton::ButtonColor style = static_cast<DeviceButton::ButtonColor>(index);
    btn1->setButtonColor(style);
    btn2->setButtonColor(style);
    btn3->setButtonColor(style);
}

//改变按钮是否可动槽函数
void Widget::on_ckCanMove_stateChanged(int arg1)
{
    bool canMove = (arg1 != 0);
    btn1->setCanMove(canMove);
    btn2->setCanMove(canMove);
    btn3->setCanMove(canMove);
}
