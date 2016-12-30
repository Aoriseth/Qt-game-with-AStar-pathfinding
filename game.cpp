#include "game.h"

game::game()
{
    pathfinding = std::make_shared<pathfinder>();
    setDestination(0,0);
}

std::shared_ptr<EnemyUnit> game::getClosestEnemy(){
    float min_cost = std::numeric_limits<float>::infinity();
    auto result = defeatableEnemies[0];
    for(auto unit:defeatableEnemies){
        setDestination(unit->getXPos(),unit->getYPos());
        bool finished  = pathfinding->calcPath_Dijkstra();
        if(finished){  //path found, enemy is reachable
            float my_cost = pathfinding->getMoveCost();
            if(my_cost<min_cost){
                min_cost = my_cost;
                result = unit;
            }
            //clear memory created by finding the closet enemy
            setStart(protagonist->getXPos(),protagonist->getYPos());
            pathfinding->setMoveCost(0.0f);
        }
        pathfinding->screen->clearPath();
    }
    return result;
}

bool game::isAllDefeated()
{
    bool flag = true;

    for(auto& unit:enemies){
        if(!unit->getDefeated()){
            flag = false;
        }
    }
    return flag;
}

std::shared_ptr<HealthModel> game::getClosestHealthpack()
{
    float min_cost = 10000;
    auto result = healthpacks[0];
    for(auto pack:healthpacks){
        setDestination(pack->getXPos(),pack->getYPos());
        bool finished  = pathfinding->calcPath_Dijkstra();
        if(finished){  //path found, health is reachable
            float my_cost = pathfinding->getMoveCost();
            if(my_cost<min_cost){
                min_cost = my_cost;
                result = pack;
            }
            //clear memory created by finding the closet enemy
            setStart(protagonist->getXPos(),protagonist->getYPos());
            pathfinding->setMoveCost(0.0f);
        }
        pathfinding->screen->clearPath();
    }

    //healthpacks.erase(result);
    return result;
}


void game::strat()
{
    while(!isAllDefeated()){ //check if all enemies are defeated
        QCoreApplication::processEvents();
        if(!goOn){return;}
        while((!isDefeatable())){//check if there is a defeatable enemy before calculate the path for closest enemy
            if(healthpacks.size()==0){
                qDebug()<<"Quit: NO healthpack left!";
                return;
            }
            if(!goForHealthpack()){
                return; //quit the loop
            }
        }
        if(!goForEnemy()){
            if(!goForHealthpack()){
                return; //quit the loop
            }
        }
    }
    setDestination(0,0);
}

void game::setView(std::shared_ptr<view> test)
{
    pathfinding->screen = test;
}

void game::setDestination(int x, int y){
    pathfinding->setXDest(x);
    pathfinding->setYDest(y);
    emit destinationChanged(x,y);
}

void game::loadWorld(QString path){
    //clear enemy and healthpack vectors
    pathfinding->clearLists();
    enemies.clear();
    healthpacks.clear();

    //load image and determine size
    QImage image(path);
    pathfinding->setXmax(image.width()-1);
    pathfinding->setYmax(image.height()-1);

    //modify number of enemies and healthpacks based on world size. If world is huge, don't generate objects
    int objectNum = (pathfinding->getXmax()+pathfinding->getYmax())/2;
    if(objectNum > 100){
        objectNum = 1;
    }

    //Convert enemies to custom EnemyUnit
    pathfinding->tiles = myWorld->createWorld(path);
    auto tempenemies = myWorld->getEnemies(objectNum);
    for(auto& unit:tempenemies){
        enemies.push_back(std::shared_ptr<EnemyUnit>(new EnemyUnit(unit->getXPos(), unit->getYPos(), unit->getValue())));
    }

    //Convert Healthpacks to custom HealthModel
    auto temphealthpacks = myWorld->getHealthPacks(objectNum);
    for(auto& pack:temphealthpacks){
        healthpacks.push_back(std::shared_ptr<HealthModel>(new HealthModel(pack->getXPos(), pack->getYPos(), pack->getValue())));
    }

    // Load protagonist and update mainwindow to display stats
    // auto tempagonist = myWorld->getProtagonist();
    protagonist = std::shared_ptr<Hero>(new Hero());

    // Display the world into a graphicsscene
    pathfinding->screen->displayWorld(image);

}

void game::setStart(int x, int y){
    protagonist->setXPos(x);
    protagonist->setYPos(y);
    pathfinding->clearLists();
    auto pos1 = std::make_shared<Tile>(std::move(*(protagonist)));
    pathfinding->myIndexes.insert((pathfinding->getXmax()+1)*y+x);
    node myNode1(pos1,nullptr); //Starting point
    pathfinding->currentNodes.enqueue(myNode1);
    myNode1.setDistance(0);
    pathfinding->availableNodes.push_back(myNode1);
}

void game::MoveProtagonist()
{
    while(pathfinding->route.size()){
        auto tile = pathfinding->route.pop();
        protagonist->setXPos((tile->getXPos()));
        protagonist->setYPos((tile->getYPos()));
        float newEnergy = protagonist->getEnergy()-1 - pathfinding->getWeight()*(1-tile->getValue());
        protagonist->updateEnergy(newEnergy);
    }
    pathfinding->clearLists();
    pathfinding->setMoveCost(0.0f);
    setStart(protagonist->getXPos(),protagonist->getYPos());
}

void game::checkMove(int xPos, int yPos){
    if(xPos>=0 && xPos<=pathfinding->getXmax() && yPos>=0 && yPos<=pathfinding->getYmax()){
        int index = yPos*(pathfinding->getXmax()+1) + xPos;
        if(!(std::isinf(pathfinding->tiles[index]->getValue()))){
            protagonist->setPos(xPos,yPos);
        }
    }

}

void game::removeHealthpack(std::shared_ptr<HealthModel> healthpack)
{
    healthpack->useHealthpack();
    int pos = find(healthpacks.begin(), healthpacks.end(), healthpack) - healthpacks.begin();
    healthpacks.erase(healthpacks.begin()+pos);
}

void game::go(int i)
{
    auto finished = false;
    switch (i) {
    case 3:
        finished = pathfinding->calcPath_BreadthFirst();
        break;
    case 4:
        finished = pathfinding->calcPath_BestFirst();
        break;
    default:
        finished = pathfinding->calcPath_Dijkstra();
        break;
    }
    // Calculate the path

    // Move the protagonist based on the calculated path
    if(finished){
            MoveProtagonist();
    }else{
        setStart(protagonist->getXPos(),protagonist->getYPos());
    }
}

bool game::goForHealthpack()
{
    auto healthpack = getClosestHealthpack();
    setDestination(healthpack->getXPos(),healthpack->getYPos());
    //pathfinding->setWeight(weight);
    bool find = pathfinding->calcPath_Dijkstra();
    if(find){
        float requiredEnergy = pathfinding->getMoveCost();
        if(requiredEnergy>protagonist->getEnergy()){
            return false; //quit the loop
        }else{
            MoveProtagonist();
            float newHealth = protagonist->getHealth()+5.0*healthpack->getValue(); //multiply by a factor of 5
            if(newHealth > 100) newHealth = 100;
            protagonist->updateHealth(newHealth);
            pathfinding->setMoveCost(0.0f);
            // Move the protagonist based on the calculated path
            removeHealthpack(healthpack);
            return true;
        }
    }else{
        qDebug()<<"Healthpack is not found!";
        return false;
    }
}

bool game::isDefeatable()
{
    defeatableEnemies.clear();
    for(auto& unit:enemies){
        if(!unit->getDefeated()){  //when a enemy is not defeated
            if(unit->getValue() < protagonist->getHealth()){ //when a enemy has a strength smaller than pro's health
                //qDebug()<<"At least one enemy is defeatable with strength: "<<(*it)->getValue();
                defeatableEnemies.push_back(unit);
            }
        }
    } //qDebug()<<"NO enemy can be defeated!";
    if(defeatableEnemies.size()>0){
        return true;
    }else{
        return false;
    }

}

bool game::goForEnemy()
{
    auto unit = getClosestEnemy();  //calculate the path and get the closest enemy
    //After the check that there is at least one defeatable enemy, which reduce the calls of getClosestEnemy();
    //the closestEnemy could still be undefeatable
    while(unit->getValue()>protagonist->getHealth()){
        qDebug()<<"Health is not enough to defeat an enemy, go for healthpack";
        if(healthpacks.size()==0){
            qDebug()<<"Quit: NO healthpack left!";
            return false;
        }
        if(!goForHealthpack()){
            return false; //quit the loop
        }
    }
    setDestination(unit->getXPos(),unit->getYPos());
    bool finished = false;
    finished = pathfinding->calcPath_Dijkstra();
    if(finished){  //Path found
        float requiredEnergy = pathfinding->getMoveCost();
        if(requiredEnergy>protagonist->getEnergy()){
            qDebug()<<"Game failed! Not enough energy to next enemy!Energy required: "<<requiredEnergy;
            setStart(protagonist->getXPos(),protagonist->getYPos());
            return false; //quit the loop
        }else{
            // Move the protagonist based on the calculated path
            MoveProtagonist();
            unit->kill();
            float newHealth = protagonist->getHealth()-unit->getValue();
            protagonist->updateHealth(newHealth);
            protagonist->updateEnergy(100);
            pathfinding->setMoveCost(0.0f);
        }
        return true;
    }else{  //Path not found
        setStart(protagonist->getXPos(),protagonist->getYPos());
        return false;
    }
}


