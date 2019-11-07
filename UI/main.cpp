#include "GMWidget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication gmApp(argc, argv);


    QCoreApplication::setOrganizationName("SimCenter");
    QCoreApplication::setApplicationName("Ground Motion Tools");

    GMWidget gmWindow;
    gmWindow.show();

    QFile styleFile(":/style.qss");
    styleFile.open( QFile::ReadOnly );
    gmApp.setStyleSheet(styleFile.readAll());
    return gmApp.exec();
}
