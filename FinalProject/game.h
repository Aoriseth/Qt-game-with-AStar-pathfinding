#ifndef GAME_H
#define GAME_H
#include <vector>
#include <memory>
#include "world.h"
#include "world_global.h"
#include <QQueue>
#include "node.h"
#include <QSet>
#include <math.h>
#include <QDebug>
#include <QStack>
#include <QGraphicsScene>



class game
{
public:
    game();
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tile>> healthpacks;
    bool breadthFirst(int x,int y);
    void clearLists();
    QQueue<node> currentNodes;
    int ymax;
    int xmax;
    QSet<int> myIndexes;
    bool calcPath();
    QStack<std::shared_ptr<Tile>> route;
    void loadWorld(QString path,QGraphicsScene * scene);
    World *myWorld = new World();
    std::unique_ptr<Protagonist> protagonist;
    void setStart(int x, int y);
    int xDest;
    int yDest;
    void setDestination(int x, int y);
private:


};

#endif // GAME_H
