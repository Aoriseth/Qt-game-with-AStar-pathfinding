#ifndef GAME_H
#define GAME_H
#include <QQueue>
#include <QDebug>
#include <QStack>
#include <QSet>
#include <QObject>
#include <QElapsedTimer>
#include <QCoreApplication>

#include <math.h>
#include <limits>
#include <vector>
#include <memory>

#include "world.h"
#include "world_global.h"
#include "node.h"
#include "enemyunit.h"
#include "healthmodel.h"
#include "hero.h"
#include "view.h"

class view;

class game: public QObject
{
    Q_OBJECT
public:
    game();

    QQueue<node> currentNodes;
    std::vector<node> availableNodes;
    std::vector<node> sptNodes;//shortest path tree
    QStack<std::shared_ptr<Tile>> route;
    QSet<int> myIndexes;

    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<EnemyUnit>> enemies;
    std::vector<std::shared_ptr<EnemyUnit>> defeatableEnemies;
    std::vector<std::shared_ptr<HealthModel>> healthpacks;

    std::shared_ptr<World> myWorld = std::make_shared<World>();
    std::shared_ptr<view> screen;
    std::shared_ptr<Hero> protagonist;

    void setView(std::shared_ptr<view> test);
    std::shared_ptr<Tile> getTile(int x,int y);
    std::shared_ptr<EnemyUnit> getClosestEnemy();
    std::shared_ptr<HealthModel> getClosestHealthpack();
    bool isAllDefeated();
    void removeHealthpack(std::shared_ptr<HealthModel> healthpack);

    int weight = 1;
    int ymax;
    int xmax;
    int xDest;
    int yDest;

    bool breadthFirst(int x,int y);
    bool calcPath_BreadthFirst();
    bool bestFirst(int x,int y);
    bool calcPath_BestFirst();
    bool Dijkstra(int x,int y);
    bool calcPath_Dijkstra();

    void clearLists();
    void strat();

    bool goOn = true;

    void setMoveCost(float value);
    float getMoveCost() const;

    void loadWorld(QString path);

    void setStart(int x, int y);

    void setDestination(int x, int y);
    int getWeight() const;
    void setWeight(int value);

    void MoveProtagonist();

    bool isDefeatable();
    bool goForHealthpack();
    bool goForEnemy();
    void go(int i);

private:
    float moveCost = 0.0f;
    node getNodeWithMinDistance();

    void breadth_addNode(int index, std::shared_ptr<node> pre);
    bool best_addNode(int x, int y, int index, std::shared_ptr<node> pre);
    void Dijkstra_addNode(int index, std::shared_ptr<node> pre, double old_dis);

    static bool node_compare(node a, node b)
    {
        return (a.getDistance() < b.getDistance());
    }

signals:
    void destinationChanged(int x, int y);
    void enemyKilled(int pos);
    void healthpackUsed(int pos);

protected slots:
    void checkMove(int xPos, int yPos);
};

#endif // GAME_H
