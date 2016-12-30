#ifndef GAME_H
#define GAME_H

#include"pathfinder.h"
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "mainwindow.h"
#include <QObject>
#include <limits>
#include "enemyunit.h"
#include "healthmodel.h"
#include "hero.h"
#include "view.h"

class view;
class pathfinder;

class game: public QObject
{
    Q_OBJECT
public:
    game();
    std::vector<std::shared_ptr<EnemyUnit>> enemies;
    std::vector<std::shared_ptr<EnemyUnit>> defeatableEnemies;
    std::vector<std::shared_ptr<HealthModel>> healthpacks;

    std::shared_ptr<pathfinder> pathfinding;
    std::shared_ptr<Hero> protagonist;
    std::shared_ptr<World> myWorld = std::make_shared<World>();
    bool goOn = true;

    void strat();    
    void loadWorld(QString path);       
    void setStart(int x, int y);
    void setDestination(int x, int y);
    void setView(std::shared_ptr<view> test);         
    void go(int i);

private:
    void MoveProtagonist();
    bool isDefeatable();
    bool isAllDefeated();
    bool goForEnemy();
    bool goForHealthpack();
    void removeHealthpack(std::shared_ptr<HealthModel> healthpack);
    std::shared_ptr<EnemyUnit> getClosestEnemy();
    std::shared_ptr<HealthModel> getClosestHealthpack();

signals:
    void destinationChanged(int x, int y);
    void enemyKilled(int pos);
    void healthpackUsed(int pos);

protected slots:
    void checkMove(int xPos, int yPos);
};

#endif // GAME_H
