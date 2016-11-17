#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "world.h"
#include "world_global.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <iostream>
#include <math.h>
#include <QGraphicsPixmapItem>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QGraphicsScene * scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

    World *myWorld = new World();
    std::vector<std::unique_ptr<Tile>> tiles = myWorld->createWorld("://resources/worldmap4.png");
    std::vector<std::unique_ptr<Enemy>> enemys = myWorld->getEnemies(10);
    std::vector<std::unique_ptr<Tile>> healthpacks = myWorld->getHealthPacks(6);
    std::unique_ptr<Protagonist> pro = myWorld->getProtagonist();

    for(auto& tile: tiles){
        int x = tile->getXPos();
        int y = tile->getYPos();
        float value = tile->getValue();
        if(std::isinf(value)) value= 255.0;
        else value = 255.0-value*255.0;
        scene->addRect(x, y, 1, 1, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 0,(int)value)));
    }

    for(auto& healthpack: healthpacks){
        int x = healthpack->getXPos();
        int y = healthpack->getYPos();

        QImage image("://resources/Supermushroom.png");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        item->setScale(0.1);
        item->setPos(x,y);
        scene->addItem(item);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
