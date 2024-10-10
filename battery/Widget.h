#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMessageBox>
#include<QString>
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
private slots:
    void initForm();
    void on_LookBtn_clicked();
};
#endif // WIDGET_H
