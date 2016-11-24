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
#include <iostream>

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

    //find my path here
    std::vector<node> currentNodes;
    auto ptr1 = std::make_shared<Tile>(std::move(*tiles[0]));
    node myNode1(ptr1,nullptr); //Starting point
    //currentNodes.push_back(myNode1);
    std::cout<<"Node1---XPos: " << myNode1.getPos()->getXPos()<<" YPos: " << myNode1.getPos()->getYPos();
    std::cout<<" Value: " << myNode1.getPos()->getValue()<<std::endl;

    auto pre2 = std::make_shared<node>(myNode1);
    auto ptr2 = std::make_shared<Tile>(std::move(*tiles[1]));
    node myNode2(ptr2,pre2);
    currentNodes.push_back(myNode2);
    std::cout<<"Node2---XPos: " << myNode2.getPos()->getXPos()<<" YPos: " << myNode2.getPos()->getYPos();
    std::cout<<" Value: " << myNode2.getPos()->getValue()<<std::endl;

    auto pre3 = std::make_shared<node>(myNode2);
    auto ptr3 = std::make_shared<Tile>(std::move(*tiles[5]));
    node myNode3(ptr3,pre3);
    currentNodes.push_back(myNode3);
    std::cout<<"Node3---XPos: " << myNode3.getPos()->getXPos()<<" YPos: " << myNode3.getPos()->getYPos();
    std::cout<<" Value: " << myNode3.getPos()->getValue()<<std::endl;

    auto pre4 = std::make_shared<node>(myNode3);
    auto ptr4 = std::make_shared<Tile>(std::move(*tiles[7]));
    node myNode4(ptr4,pre4);
    currentNodes.push_back(myNode4);
    std::cout<<"Node3---XPos: " << myNode4.getPos()->getXPos()<<" YPos: " << myNode4.getPos()->getYPos();
    std::cout<<" Value: " << myNode4.getPos()->getValue()<<std::endl;

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
