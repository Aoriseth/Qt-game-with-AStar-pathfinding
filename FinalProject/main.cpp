#include "mainwindow.h"
#include <QApplication>
#include "lib/world.h"
#include "lib/world_global.h"
#include "game.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    game* logic = new game();
    view* screen = new view();

    logic->clearLists();
    screen->setLogic(logic);
    logic->setView(screen);
    w.setLogic(logic);
    w.setView(screen);
    w.initconnectors();

    w.show();


    return a.exec();
}
