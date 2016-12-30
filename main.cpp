#include "mainwindow.h"
#include <QApplication>
#include "lib/world.h"
#include "lib/world_global.h"
#include "game.h"
#include "view.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    std::shared_ptr<MainWindow> w = std::make_shared<MainWindow>();
    std::shared_ptr<game> logic = std::make_shared<game>();
    std::shared_ptr<view> screen = std::make_shared<view>();

    screen->setLogic(logic);
    logic->setView(screen);
    w->setLogic(logic);
    w->setView(screen);

    w->show();
    return a.exec();
}
