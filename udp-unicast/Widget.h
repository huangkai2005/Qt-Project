#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMessageBox>
#include <QHostAddress>
#include <QUdpSocket>//网络通信对象
#include <QString>
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

private slots:

    void on_SendBtn_clicked();

    void on_btn_clicked();

private:
    Ui::Widget *ui;
    QUdpSocket *MSocket;
};
#endif // WIDGET_H
