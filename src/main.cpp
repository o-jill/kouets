#include "mainwindow.h"
#include <QApplication>

#include "kouetsapp.h"

int main(int argc, char *argv[])
{
    KouetsApp a(argc, argv);
    a.LoadIni();

    MainWindow w;
    w.show();

    return a.exec();
}
