#include "mainwindow.h"
#include <QApplication>
#include "fl/Headers.h"
#include "fl/fuzzylite.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
