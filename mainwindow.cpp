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
#include "node.h"
#include <vector>

void addItemToScene(QImage image, int x, int y);
QGraphicsScene * scene;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(0,0,256/5,256/5,Qt::KeepAspectRatio);

    World *myWorld = new World();
    QString path = "://resources/maze_5_5.png";
    std::vector<std::unique_ptr<Tile>> tiles = myWorld->createWorld(path);
    std::vector<std::unique_ptr<Enemy>> enemies = myWorld->getEnemies(2);
    std::vector<std::unique_ptr<Tile>> healthpacks = myWorld->getHealthPacks(2);
    std::unique_ptr<Protagonist> pro = myWorld->getProtagonist();

    for(auto& tile: tiles){
        int x = tile->getXPos();
        int y = tile->getYPos();
        float value = tile->getValue();
        if(std::isinf(value)) value= 255.0;
        else value = 255.0-value*255.0;
        scene->addRect(256*x, 256*y, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 0,(int)value)));
    }

    QImage image("://resources/Pixel-mario.png");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setScale(1);
    item->setPos(pro->getXPos(),pro->getYPos());
    scene->addItem(item);

    for(auto& healthpack: healthpacks){
        int x = healthpack->getXPos();
        int y = healthpack->getYPos();
        QImage image("://resources/Supermushroom.png");
        addItemToScene(image, x, y);
    }
    for(auto& enemy: enemies){
        int x = enemy->getXPos();
        int y = enemy->getYPos();
        QImage image("://resources/goomba.gif");
        addItemToScene(image, x, y);
    }



}

void addItemToScene(QImage image, int x, int y){
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setScale(1);
    item->setPos(256*x,256*y);
    scene->addItem(item);
}


MainWindow::~MainWindow()
{
    delete ui;
}
