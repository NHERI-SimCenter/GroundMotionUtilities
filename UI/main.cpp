#include "GMWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GMWidget w;
    w.show();

    return a.exec();
}
