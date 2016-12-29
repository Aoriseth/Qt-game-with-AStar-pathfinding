#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "game.h"
#include "scene.h"
#include "enemyview.h"
#include <QObject>

class game;

class view: public QObject
{
    Q_OBJECT
public:
    view();
    Scene* sceneView;
    std::shared_ptr<game> logic;
    QGraphicsItem * destView;
    std::vector<QGraphicsItem*> pathView;
    void addPathStep(int x,int y);
    void clearPath();
    void addItemToScene(QGraphicsPixmapItem *item, int x, int y);
    void setLogic(std::shared_ptr<game> pass);
    void showEnemies();
    void showHealthpacks();
    void showProtagonist();
    void setProtagonistPosition(int x, int y);
    void clearLists();

private:
    QGraphicsPixmapItem* protagonistView;
    std::vector<EnemyView*> enemyItems;
    std::vector<QGraphicsPixmapItem*> healthpackItems;
signals:
    void updateViewport();
};

#endif // VIEW_H
