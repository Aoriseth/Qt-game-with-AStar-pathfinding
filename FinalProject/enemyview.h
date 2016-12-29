#ifndef ENEMYVIEW_H
#define ENEMYVIEW_H
#include "enemyunit.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <scene.h>

class EnemyUnit;

#include <QObject>

class EnemyView: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    EnemyView();
    EnemyView(QPixmap pm, Scene* vPass, std::shared_ptr<EnemyUnit> mPass);
    void addToScene();

private:
    Scene* view;
    std::shared_ptr<EnemyUnit> mEnemy;
protected slots:
    void updateVisual();
};

#endif // ENEMYVIEW_H
