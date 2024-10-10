#include "Widget.h"
#include <QApplication>
#include <QFont>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("Microsoft Yahei");
    font.setPixelSize(13);
    a.setFont(font);

    Widget w;
    w.setWindowTitle("Demo");
    w.show();

    return a.exec();
}
