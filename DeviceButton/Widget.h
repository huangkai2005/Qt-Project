#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QVector>
#include<QPushButton>
#include"DeviceButton.h"
#include<QDebug>

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:
    Ui::Widget *ui;
    DeviceButton *btn1;
    DeviceButton *btn2;
    DeviceButton *btn3;
    QVector<QPushButton *> btnStyle;
    QVector<QPushButton *> btnColor;

private slots:
    void initForm();
    void changeStyle();
    void changeColor();
    void on_ckCanMove_stateChanged(int arg1);

};
#endif // WIDGET_H
