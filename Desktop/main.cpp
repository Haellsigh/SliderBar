#include "mainwindow.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QCoreApplication::setOrganizationName("Nelsoft");
    QCoreApplication::setApplicationName("SlideBar");

    QApplication a(argc, argv);

    MainWindow w;
    w.show();

    return a.exec();
}
