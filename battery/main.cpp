#include "Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPixelSize(13);
    a.setFont(font);

    Widget w;
    w.setWindowTitle("电池电量控件");
    w.show();

    return a.exec();
}
