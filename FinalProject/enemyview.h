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
    EnemyView(QPixmap pm, std::shared_ptr<Scene> vPass, std::shared_ptr<EnemyUnit> mPass);
    void addToScene();

private:
    std::shared_ptr<Scene> view;
    std::shared_ptr<EnemyUnit> mEnemy;
    std::shared_ptr<QGraphicsTextItem> text;
protected slots:
    void updateVisual();
};

#endif // ENEMYVIEW_H
