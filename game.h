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
#include <QGraphicsItem>
#include "mainwindow.h"
#include "view.h"
#include <QObject>
#include <limits>
#include "enemyunit.h"
#include "healthmodel.h"



class game: public QObject
{
    Q_OBJECT
public:
    game();
    QQueue<node> currentNodes;
    std::vector<node> availableNodes;
    std::vector<node> sptNodes;//shortest path tree
    std::vector<std::unique_ptr<Tile>> tiles;
    std::vector<std::shared_ptr<EnemyUnit>> enemies;
    std::vector<std::shared_ptr<EnemyUnit>> defeatableEnemies;
    std::vector<std::shared_ptr<HealthModel>> healthpacks;
    bool breadthFirst(int x,int y);
    bool bestFirst(int x,int y);
    bool AStar(int x,int y);
    void clearLists();
    void strat();

    int weight = 1;
    int ymax;
    int xmax;
    QSet<int> myIndexes;
    bool calcPath_BreadthFirst();
    bool calcPath_BestFirst();
    bool calcPath_AStar();
    QStack<std::shared_ptr<Tile>> route;
    void loadWorld(QString path);
    std::shared_ptr<World> myWorld = std::make_shared<World>();
    void setStart(int x, int y);
    int xDest;
    int yDest;
    void setDestination(int x, int y);
    int getWeight() const;
    void setWeight(int value);
    void setView(std::shared_ptr<view> test);
    std::shared_ptr<view> screen;
    std::shared_ptr<Tile> getTile(int x,int y);
    std::shared_ptr<EnemyUnit> getClosestEnemy();
    bool isAllDefeated();
    std::shared_ptr<HealthModel> getClosestHealthpack();
    float moveCost = 0.0f;

    float getMoveCost() const;
    void setMoveCost(float value);

    void MoveProtagonist();
    int getProtagonistX();
    int getProtagonistY();
    void setEnergy(float passEnergy);
    void setHealth(float passHealth);
    float getEnergy();
    float getHealth();
    std::shared_ptr<Protagonist> getProtagonist();

    void removeHealthpack(std::shared_ptr<HealthModel> healthpack);
    bool goForHealthpack();
    bool isDefeatable();
    bool goForEnemy();

    void MoveProLeft();
    void MoveProRight();
    void MoveProUp();
    void MoveProDown();
    void go();


private:
    std::shared_ptr<Protagonist> protagonist;
    void checkAndSetPos(int xPos, int yPos);    
    void breadth_addNode(int index, std::shared_ptr<node> pre);
    bool best_addNode(int x, int y, int index, std::shared_ptr<node> pre);
    void aStar_addNode(int index, std::shared_ptr<node> pre, double old_dis);

    static bool node_compare(node a, node b)
    {
        return (a.getDistance() < b.getDistance());
    }
    node getNodeWithMinDistance();

signals:
    void enemyKilled(int pos);
    void healthpackUsed(int pos);
    void changeStats(float energy, float health);

protected slots:
    void movePro(int x);
};

#endif // GAME_H
