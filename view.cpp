#include "view.h"
#include <QGraphicsSceneMouseEvent>


view::view()
{

}

void view::addPathStep(int x, int y)
{
   auto step = sceneView->addRect(0, 0, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255)));
   step->setZValue(1);
   step->setOpacity(0.5);
   step->setScale(0.00390625);
   step->setPos(x,y);

   pathView.push_back(step);
//   emit updateViewport();
}

void view::clearPath()
{
    for(QGraphicsItem* step: pathView){
        delete step;
    }
    pathView.clear();
    emit updateViewport();
}

void view::setLogic(std::shared_ptr<game> pass)
{
    logic = pass;
}

void view::showEnemies()
{
    for(auto enemy: logic->enemies){
        auto item = new EnemyView(QPixmap(":/resources/goomba.gif"),sceneView,enemy);
        item->addToScene();
        connect(enemy.get(), SIGNAL(defeat()), item, SLOT(updateVisual()));
        connect(enemy.get(), SIGNAL(defeat()), this, SIGNAL(updateViewport()));
        enemyItems.push_back(item);
    }
}

void view::showHealthpacks()
{
    for(auto healthpack: logic->healthpacks){
        HealthView* item = new HealthView(QPixmap(":/resources/Supermushroom.png"),sceneView,healthpack);
        item->addToScene();
        connect(healthpack.get(), SIGNAL(used()), item, SLOT(healthUsed()));
        healthpackItems.push_back(item);
    }
}

void view::showProtagonist(){
    QImage image(":/resources/Pixel-mario.png");
    protagonistView = std::make_shared<QGraphicsPixmapItem>(QPixmap::fromImage(image));
    protagonistView->setZValue(3);
    protagonistView->setScale(0.00390625);
    protagonistView->setPos(logic->getProtagonistX(),logic->getProtagonistY());
    sceneView->addItem(protagonistView.get());
}

void view::setProtagonistPosition(int x, int y)
{
    protagonistView->setPos(x,y);
}

void view::clearLists()
{
    enemyItems.clear();
    healthpackItems.clear();
}

