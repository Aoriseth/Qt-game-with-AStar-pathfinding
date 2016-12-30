#ifndef POISONVIEW_H
#define POISONVIEW_H
#include "world.h"
#include <QObject>
#include <QGraphicsPixmapItem>
#include "scene.h"

class PoisonView: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    PoisonView(QPixmap pm, std::shared_ptr<Scene> vPass, std::shared_ptr<PEnemy> mPass);
};

#endif // POISONVIEW_H
