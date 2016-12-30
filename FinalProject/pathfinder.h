#ifndef PATHFINDER_H
#define PATHFINDER_H
#include "world.h"
#include "world_global.h"
#include <vector>
#include <memory>
#include <QQueue>
#include "node.h"
#include <QSet>
#include <math.h>
#include <QDebug>
#include <QStack>
#include "view.h"
class view;

class pathfinder
{
public:
    pathfinder();
    std::vector<std::unique_ptr<Tile>> tiles;
    QQueue<node> currentNodes;
    std::vector<node> availableNodes;
    std::vector<node> sptNodes;//shortest path tree
    QSet<int> myIndexes;
    QStack<std::shared_ptr<Tile>> route;
    std::shared_ptr<view> screen;

    void clearLists();
    bool calcPath_BreadthFirst(int xDes, int yDes);
    bool calcPath_BestFirst(int xDes, int yDes);
    bool calcPath_Dijkstra(int xDes, int yDes);

    int getWeight() const;
    void setWeight(int value);

    int getYmax() const;
    void setYmax(int value);

    int getXmax() const;
    void setXmax(int value);

    float getMoveCost() const;
    void setMoveCost(float value);

private:
    int weight = 1;
    int ymax = 0;
    int xmax = 0;
    float moveCost = 0.0f;
    bool breadthFirst(int x,int y);
    bool bestFirst(int x,int y);
    bool Dijkstra(int x,int y);
    void breadth_addNode(int index, std::shared_ptr<node> pre);
    bool best_addNode(int x, int y, int index, std::shared_ptr<node> pre);
    void Dijkstra_addNode(int index, std::shared_ptr<node> pre, double old_dis);
    node getNodeWithMinDistance();
    std::shared_ptr<Tile> getTile(int x,int y);
    static bool node_compare(node a, node b)
    {
        return (a.getDistance() < b.getDistance());
    }
};

#endif // PATHFINDER_H
