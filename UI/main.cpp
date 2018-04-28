#include "GMWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication gmApp(argc, argv);
    GMWidget gmWindow;
    gmWindow.show();

    QFile styleFile(":/style.qss");
    styleFile.open( QFile::ReadOnly );
    gmApp.setStyleSheet(styleFile.readAll());
    return gmApp.exec();
}
