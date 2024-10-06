#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QUdpSocket>
#include<QMessageBox>
#include<QDateTime>
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
    void on_JoinBtn_clicked();

    void on_UnjoinBtn_clicked();

    void on_pushButton_clicked();

    void on_SendBtn_clicked();

private:
    Ui::Widget *ui;

    QUdpSocket * MSocket;
};
#endif // WIDGET_H
