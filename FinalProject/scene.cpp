#include "scene.h"

Scene::Scene(QObject *parent):
    QGraphicsScene(parent)
{
    this->setBackgroundBrush(Qt::black);
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    auto point = mouseEvent->scenePos();
    int x = floor(point.x());
    int y = floor(point.y());
    qDebug()<<"selected x: " << x << " y: " << y;
    emit locationClicked(x,y);

}

void locationClicked(int x, int y){

}
