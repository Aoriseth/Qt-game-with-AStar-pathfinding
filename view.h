#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "game.h"
#include "scene.h"

class game;

class view
{
public:
    view();
    Scene * sceneView;
    game* logic;
    QGraphicsItem * destView;
    std::vector<QGraphicsItem*> pathView;

    void addItemToScene(QGraphicsPixmapItem *item, int x, int y);
    void setLogic(game* pass);
    void showEnemies();
    void showHealthpacks();
    void showProtagonist();
    void setProtagonistPosition(int x, int y);

private:
    QGraphicsPixmapItem* protagonistView;
    std::vector<QGraphicsPixmapItem*> enemyItems;
    std::vector<QGraphicsPixmapItem*> healthpackItems;
};

#endif // VIEW_H
