#include "mainwindow.h"
#include <QApplication>
#include "world.h"
#include "world_global.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    World *myWorld = new World();//(100,100,QImage(":/new/pictures/resources/worldmap4.png"));
    std::vector<std::unique_ptr<Tile>> tiles = myWorld->createWorld(":/new/pictures/resources/worldmap4.png");
    std::vector<std::unique_ptr<Enemy>> enemys = myWorld->getEnemies(10);
    std::vector<std::unique_ptr<Tile>> healthpacks = myWorld->getHealthPacks(6);
    std::unique_ptr<Protagonist> pro = myWorld->getProtagonist();
    //w.

    return a.exec();
}
