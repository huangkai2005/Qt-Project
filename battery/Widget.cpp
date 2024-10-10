#include "Widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->initForm();
    ui->Slider->setMaximum(100);//设置滑条最大值为100，防止出现电池电量达不到100的问题
}

Widget::~Widget()
{
    delete ui;
}
//初始化格局
void Widget::initForm()
{
    connect(ui->Slider, &QSlider::valueChanged, ui->battery, static_cast<void (Battery::*)(int)>(&Battery::setValue));
    ui->Slider->setValue(0);//初始化电池电量
}
void Widget::on_LookBtn_clicked()
{
    QMessageBox::information(this, "提示", "当前电量为 " + QString::number(ui->battery->getValue()) + "%");
}


