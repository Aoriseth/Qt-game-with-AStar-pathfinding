#include "view.h"
#include <QGraphicsSceneMouseEvent>


view::view()
{

}

void view::addItemToScene(QGraphicsPixmapItem* item, int x, int y){
    //auto item = std::make_shared<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap::fromImage(image)));
    item->setScale(0.00390625);
    item->setPos(x,y);
    sceneView->addItem(item);
}

void view::setLogic(game *pass)
{
    logic = pass;
}

void view::showEnemies()
{
    for(auto& enemy: logic->enemies){
        int x = enemy->getXPos();
        int y = enemy->getYPos();
        QImage image(":/resources/goomba.gif");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        addItemToScene(item, x, y);
        enemyItems.push_back(item);
    }
}

void view::showHealthpacks()
{
    for(auto& healthpack: logic->healthpacks){
        int x = healthpack->getXPos();
        int y = healthpack->getYPos();
        QImage image(":/resources/Supermushroom.png");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        addItemToScene(item, x, y);
        healthpackItems.push_back(item);
    }
}

void view::showProtagonist(){
    QImage image(":/resources/Pixel-mario.png");
    protagonistView = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    protagonistView->setZValue(3);
    protagonistView->setScale(0.00390625);
    protagonistView->setPos(logic->getProtagonistX(),logic->getProtagonistY());
    sceneView->addItem(protagonistView);
}

void view::setProtagonistPosition(int x, int y)
{
    protagonistView->setPos(x,y);
}
