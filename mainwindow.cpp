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
#include <QDebug>
#include <QSet>
#include <algorithm>
#include <QStack>
#include <chrono>
#include <thread>
#include <QFileDialog>

QGraphicsScene * scene;

World *myWorld = new World();
QString path = ":/resources/maze_5_5.png";
std::vector<std::unique_ptr<Tile>> tiles;
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<std::unique_ptr<Tile>> healthpacks;
std::unique_ptr<Protagonist> pro;

QQueue<node> currentNodes;
QStack<std::shared_ptr<Tile>> route;
QSet<int> myIndexes;
QGraphicsPixmapItem* mario;
auto xmax = 0;
auto ymax = 0;

void addItemToScene(QImage image, int x, int y);
bool findMyPath(int x,int y);

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    scene = new QGraphicsScene(this);
    ui->graphicsView->setScene(scene);

}

void addItemToScene(QImage image, int x, int y){
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    item->setScale(1);
    item->setPos(256*x,256*y);
    scene->addItem(item);
}

void drawScene(){


    while((!findMyPath(0,4)) &&(currentNodes.size())){
        //currentNodes.dequeue();
        qDebug()<< "Path not found yet!"<<" Queue Size = "<<currentNodes.size();
    }
    if(currentNodes.size()){
        qDebug()<< "Path found !!!!!";
        node destination = currentNodes.head();
        for(;destination.getPre()!=nullptr;){
            route.push(destination.getTile());
            destination=*(destination.getPre());
            qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();
        }
    }else{
        qDebug()<< "Path is not found in the end!!!!!";
    }



}




bool findMyPath(int x,int y){
    QListIterator<node> i(currentNodes);
    while(i.hasNext()) {
        const node myNode = i.next();
        auto tile = myNode.getTile();
        if((tile->getXPos()==x) && (tile->getYPos()==y)){
            return true;
        }else{

                if(tile->getXPos()<xmax){
                    int index = (xmax+1)*(tile->getYPos()) + tile->getXPos() + 1;
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }
                if(tile->getXPos()>0){
                    auto index = (xmax+1)*(tile->getYPos()) + tile->getXPos() - 1;
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }
                if(tile->getYPos()<ymax){
                    int index = (tile->getYPos() +1)*(xmax+1) + tile->getXPos();
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }
                if(tile->getYPos()>0){
                    int index = (tile->getYPos() -1)*(xmax+1) + tile->getXPos();
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
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

void MainWindow::play_clicked()
{
    drawScene();
    while(route.size()){
        auto tile = route.pop();
        mario->setPos(256*(tile->getXPos()),256*(tile->getYPos()));
        ui->graphicsView->viewport()->repaint();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

}
void MainWindow::OpenMap()
{


    scene->clear();
    scene->setSceneRect(scene->itemsBoundingRect());
    QString fileName = QFileDialog::getOpenFileName(this,tr("Select map"));
    path = fileName;

    tiles = myWorld->createWorld(path);
    enemies = myWorld->getEnemies(2);
    healthpacks = myWorld->getHealthPacks(2);
    pro = myWorld->getProtagonist();


    for(auto& tile: tiles){
        int x = tile->getXPos();
        if (x>xmax){
            xmax=x;
        }
        int y = tile->getYPos();
        if (y>ymax){
            ymax=y;

        }
        float value = tile->getValue();
        if(std::isinf(value)) value= 255.0;
        else value = 255.0-value*255.0;
        scene->addRect(256*x, 256*y, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 0,(int)value)));
    }
    qDebug()<<"ymax is "<<ymax;
    qDebug()<<"xmax is "<<xmax;

    ui->graphicsView->fitInView(0,0,350*xmax,350*ymax,Qt::KeepAspectRatio);

    QImage image(":/resources/Pixel-mario.png");
    mario = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    mario->setZValue(3);
    mario->setScale(1);
    mario->setPos(pro->getXPos(),pro->getYPos());
    scene->addItem(mario);

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
    myIndexes.insert(0);
    node myNode1(pos1,nullptr); //Starting point
    currentNodes.enqueue(myNode1);
//    std::cout<<"Node1---XPos: " << myNode1.getPos()->getXPos()<<" YPos: " << myNode1.getPos()->getYPos();
//    std::cout<<" Value: " << myNode1.getPos()->getValue()<<std::endl;

    auto item = scene->addRect(256*0, 256*4, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255)));
    item->setFlag(QGraphicsItem::ItemIsSelectable, true);

}
