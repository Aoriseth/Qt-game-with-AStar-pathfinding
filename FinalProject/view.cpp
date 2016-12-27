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
