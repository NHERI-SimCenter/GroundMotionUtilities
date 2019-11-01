#include "PEERNGAW2Widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PEERNGAW2Widget w;
    w.show();

    return a.exec();
}
