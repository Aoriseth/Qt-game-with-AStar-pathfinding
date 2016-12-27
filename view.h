#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "game.h"

class game;

class view
{
public:
    view();
    QGraphicsScene * scene;
    game* logic;
    void addItemToScene(QGraphicsPixmapItem *item, int x, int y);
    void setLogic(game* pass);

    void showProtagonist();
    void setProtagonistPosition(int x, int y);

private:
    QGraphicsPixmapItem* protagonistView;
};

#endif // VIEW_H
