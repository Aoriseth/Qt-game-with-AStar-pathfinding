#include "mainwindow.h"
#include <QApplication>
#include "lib/world.h"
#include "lib/world_global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
