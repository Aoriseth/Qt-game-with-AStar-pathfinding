#include "scene.h"

Scene::Scene(QObject *parent):
    QGraphicsScene(parent)
{

    this->setBackgroundBrush(Qt::black);
}

Scene::~Scene()
{
    qDebug()<<"Scene destroyed";
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
    auto point = mouseEvent->scenePos();
    int x = floor(point.x());
    int y = floor(point.y());
    emit locationClicked(x,y);


}

void Scene::keyPressEvent(QKeyEvent *event)
 {
     QGraphicsScene::keyPressEvent(event);
     if(event->key() == Qt::Key_Left)
        {
            emit keyClicked(1);
//            qDebug() << "Move Left";
        }
     if(event->key() == Qt::Key_Right)
        {
            emit keyClicked(2);
//            qDebug() << "Move Right";
        }
     if(event->key() == Qt::Key_Up)
        {
            emit keyClicked(3);
//            qDebug() << "Move Up";
        }
     if(event->key() == Qt::Key_Down)
        {
            emit keyClicked(4);
//            qDebug() << "Move Down";
        }

 }

