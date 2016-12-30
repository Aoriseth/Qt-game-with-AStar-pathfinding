#include "view.h"
#include <QGraphicsSceneMouseEvent>


view::view()
{

}

void view::addPathStep(int x, int y)
{
   // Create a red rectangle when a step is added and join it to the pathView vector
   auto step = std::shared_ptr<QGraphicsItem>(sceneView->addRect(0, 0, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255))));
   step->setZValue(1);
   step->setOpacity(0.5);
   step->setScale(0.00390625);
   step->setPos(x,y);

   pathView.push_back(step);
}

void view::clearPath()
{
    // will clear the path overlay and refreshes the viewport
    pathView.clear();
    emit updateViewport();
}

void view::setLogic(std::shared_ptr<game> pass)
{
    logic = pass;
}

void view::showEnemies()
{
    // Create EnemyViews for each enemy and link their signals to the corresponding EnemyUnit
    for(auto enemy: logic->enemies){
        auto item = new EnemyView(QPixmap(":/resources/goomba.gif"),sceneView,enemy);
        item->addToScene();
        connect(enemy.get(), SIGNAL(defeat(bool)), item, SLOT(updateVisual(bool)));
        connect(enemy.get(), SIGNAL(poisoning()), item, SLOT(affectArea()));
        connect(enemy.get(), SIGNAL(defeat(bool)), this, SIGNAL(updateViewport()));
        enemyItems.push_back(item);
    }
}

void view::showHealthpacks()
{
    // Create HealthViews for each healthpack and link their signals to the corresponding HealthModel
    for(auto healthpack: logic->healthpacks){
        HealthView* item = new HealthView(QPixmap(":/resources/Supermushroom.png"),sceneView,healthpack);
        item->addToScene();
        connect(healthpack.get(), SIGNAL(used()), item, SLOT(healthUsed()));
        healthpackItems.push_back(item);
    }
}

void view::showProtagonist(){
    // Add the protagonist to the graphicsscene
    QImage image(":/resources/Pixel-mario.png");
    protagonistView = std::make_shared<QGraphicsPixmapItem>(QPixmap::fromImage(image));
    protagonistView->setZValue(3);
    protagonistView->setScale(0.00390625);
    protagonistView->setPos(logic->protagonist->getXPos(),logic->protagonist->getYPos());
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

void view::initDestination()
{
    destView.reset();
    destView = std::shared_ptr<QGraphicsItem>(sceneView->addRect(0, 0, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 255,255))));
    destView->setScale(0.00390625);
    destView->setOpacity(0.5);
}

std::shared_ptr<QGraphicsPixmapItem> view::getWorldView() const
{
    return worldView;
}

void view::displayWorld(QImage image)
{
    destView.reset();
    worldView = std::make_shared<QGraphicsPixmapItem>(QPixmap::fromImage(image));
    worldView->setZValue(0);
    worldView->setScale(1);
    worldView->setPos(0,0);
    sceneView->addItem(worldView.get());
}

void view::indicateDestination(int x, int y)
{
    destView->setPos(x,y);
    emit updateViewport();
}
