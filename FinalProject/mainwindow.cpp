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
#include <QString>
#include <QListIterator>
#include <QQueue>

QGraphicsScene * scene;

World *myWorld = new World();
QString path = ":/resources/maze_5_5.png";
std::vector<std::unique_ptr<Tile>> tiles;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<std::unique_ptr<Tile>> healthpacks;
std::unique_ptr<Protagonist> pro;

QQueue<node> currentNodes;

void addItemToScene(QImage image, int x, int y);
bool findMyPath();

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->fitInView(0,0,256/5,256/5,Qt::KeepAspectRatio);

    tiles = myWorld->createWorld(path);
    enemies = myWorld->getEnemies(2);
    healthpacks = myWorld->getHealthPacks(2);
    pro = myWorld->getProtagonist();

    for(auto& tile: tiles){
        int x = tile->getXPos();
        int y = tile->getYPos();
        float value = tile->getValue();
        if(std::isinf(value)) value= 255.0;
        else value = 255.0-value*255.0;
        scene->addRect(256*x, 256*y, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 0,(int)value)));
    }

    QImage image(":/resources/Pixel-mario.png");
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setScale(1);
    item->setPos(pro->getXPos(),pro->getYPos());
    scene->addItem(item);

    for(auto& healthpack: healthpacks){
        int x = healthpack->getXPos();
        int y = healthpack->getYPos();
        QImage image1(":/resources/Supermushroom.png");
        addItemToScene(image1, x, y);
    }
    for(auto& enemy: enemies){
        int x = enemy->getXPos();
        int y = enemy->getYPos();
        QImage image2(":/resources/goomba.gif");
        addItemToScene(image2, x, y);
    }

    auto pos1 = std::make_shared<Tile>(std::move(*(tiles[0])));
    node myNode1(pos1,nullptr); //Starting point
    currentNodes.enqueue(myNode1);
//    std::cout<<"Node1---XPos: " << myNode1.getPos()->getXPos()<<" YPos: " << myNode1.getPos()->getYPos();
//    std::cout<<" Value: " << myNode1.getPos()->getValue()<<std::endl;

    while(findMyPath()){
        std::cout<< "Path found !!!!!"<<std::endl;
    }

}

void addItemToScene(QImage image, int x, int y){
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setScale(1);
    item->setPos(256*x,256*y);
    scene->addItem(item);
}

bool findMyPath(){
    QListIterator<node> i(currentNodes);
    while(i.hasNext()) {
        auto tile = (i.next()).getTile();
        if((tile->getXPos()>=5) && (tile->getYPos()>=5)){
            return true;
        }else{
            if(tile->getXPos()<5){
                int index = 5*(tile->getYPos()) + tile->getXPos() + 1;
                auto pre4 = std::make_shared<node>(i.next());//Something went wrong here
                auto pos4 = std::make_shared<Tile>(std::move(*tiles[index]));
                node myxNode(pos4,pre4);
                currentNodes.enqueue(myxNode);
            }
            if(tile->getYPos()<5){
                int index = (tile->getYPos() +1)*5 + tile->getXPos();
                auto pre4 = std::make_shared<node>(i.next());
                auto pos4 = std::make_shared<Tile>(std::move(*tiles[index]));
                node myyNode(pos4,pre4);
                currentNodes.enqueue(myyNode);
            }
            currentNodes.dequeue();
        }
    }
    return false;
}

MainWindow::~MainWindow()
{
    delete ui;
}
