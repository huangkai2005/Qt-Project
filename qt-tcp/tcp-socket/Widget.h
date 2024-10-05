#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtNetwork/QTcpServer>//通信对象
#include<QtNetwork/QTcpSocket>//网络套接字
#include<QtNetwork/QHostAddress>
#include<QMessageBox>
#include <QNetworkProxy>
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

    void on_SocketBtn_clicked();

    void on_SendBtn_clicked();


private:
    Ui::Widget *ui;
    QTcpSocket * MySocket;
};
#endif // WIDGET_H
