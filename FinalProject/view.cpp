#include "view.h"



view::view()
{

}

void view::addItemToScene(QGraphicsPixmapItem* item, int x, int y){
    //auto item = std::make_shared<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap::fromImage(image)));
    item->setScale(1);
    item->setPos(256*x,256*y);
    scene->addItem(item);
}

void view::setLogic(game *pass)
{
    logic = pass;
}

void view::showProtagonist(){
    QImage image(":/resources/Pixel-mario.png");
    protagonistView = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    protagonistView->setZValue(3);
    protagonistView->setPos(logic->getProtagonistX(),logic->getProtagonistY());
    scene->addItem(protagonistView);
}

void view::setProtagonistPosition(int x, int y)
{
    protagonistView->setPos(256*x,256*y);
}
