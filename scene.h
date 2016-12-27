#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <math.h>

class Scene: public QGraphicsScene
{
    Q_OBJECT
public:
    explicit Scene(QObject *parent = 0);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);

signals:
    void locationClicked(int x, int y);
};

#endif // SCENE_H
