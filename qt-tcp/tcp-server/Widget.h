#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QtXml/QDomDocument>
#include<QtNetwork/QHostInfo>
#include<QtNetwork/QTcpServer> //服务器
#include<QtNetwork/QTcpSocket> //网络通信对象（套接字）
#include<QMessageBox>
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
    void on_HostBtn_clicked();

    void on_MBtn_clicked();

    void on_SendBtn_clicked();

private:
    Ui::Widget *ui;
    QTcpServer * MyServer;
    QTcpSocket * MySocket;//套接字
    int MPort;
};
#endif // WIDGET_H
