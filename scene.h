#ifndef SCENE_H
#define SCENE_H
#include <QGraphicsScene>
#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <math.h>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QObject>

class Scene: public QGraphicsScene
{
    Q_OBJECT
public:
    Scene(QObject* parent = 0);
    virtual ~Scene() = default;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent);
    virtual void keyPressEvent(QKeyEvent *event);


signals:
    void locationClicked(int x, int y);
    void keyClicked(int x);
};

#endif // SCENE_H
