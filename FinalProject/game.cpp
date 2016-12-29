#include "game.h"

game::game()
{
    ymax = 0;
    xmax= 0;
    setDestination(0,0);
}

void game::breadth_addNode(int index, std::shared_ptr<node> pre){
    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
        if(!(std::isinf(tiles[index]->getValue()))){
            node myxNode(pos,pre);
            currentNodes.enqueue(myxNode);
            myIndexes.insert(index);
        }
    }
}

bool game::breadthFirst(int x,int y){
    QListIterator<node> i(currentNodes);
    while(i.hasNext()) {
        const node myNode = i.next();
        auto tile = myNode.getTile();
        auto pre = std::make_shared<node>(myNode);
        if((tile->getXPos()==x) && (tile->getYPos()==y)){
            return true;
        }else{            
            if(tile->getXPos()<xmax){
                int index = (xmax+1)*(tile->getYPos()) + tile->getXPos() + 1;
                breadth_addNode(index, pre);
            }
            if(tile->getXPos()>0){
                auto index = (xmax+1)*(tile->getYPos()) + tile->getXPos() - 1;
                breadth_addNode(index, pre);
            }
            if(tile->getYPos()<ymax){
                int index = (tile->getYPos() +1)*(xmax+1) + tile->getXPos();
                breadth_addNode(index, pre);
            }
            if(tile->getYPos()>0){
                int index = (tile->getYPos() -1)*(xmax+1) + tile->getXPos();
                breadth_addNode(index, pre);
            }
            currentNodes.dequeue();
        }
    }
    return false;
}

bool game::best_addNode(int x, int y, int index, std::shared_ptr<node> pre){
    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
        if(!(std::isinf(tiles[index]->getValue()))){
            auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
            node myxNode(pos,pre);
            double distance =pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2);
            myxNode.setDistance(distance);
            availableNodes.push_back(myxNode); //qDebug()<<"Add new node with index:"<<index;
            myIndexes.insert(index);//qDebug()<<"Distance:"<<distance;
            if(distance==0)
                return true;
        }
    }
    return false;
}

bool game::bestFirst(int x, int y)
{
    do{
        node myNode = getNodeWithMinDistance();
        auto myTile = myNode.getTile();
        auto pre = std::make_shared<node>(myNode);
        if(myTile->getXPos()<xmax){
            int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() + 1;
            bool isFinish = best_addNode(x, y, index, pre);
            if(isFinish) {return true;}
        }
        if(myTile->getXPos()>0){
            int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() - 1;
            bool isFinish = best_addNode(x, y, index, pre);
            if(isFinish) {return true;}
        }
        if(myTile->getYPos()<ymax){
            int index = (myTile->getYPos() +1)*(xmax+1) + myTile->getXPos();
            bool isFinish = best_addNode(x, y, index, pre);
            if(isFinish) {return true;}
        }
        if(myTile->getYPos()>0){
            int index = (myTile->getYPos() -1)*(xmax+1) + myTile->getXPos();
            bool isFinish = best_addNode(x, y, index, pre);
            if(isFinish) {return true;}
        }
        //qDebug()<<"Size:"<<availableNodes.size();
    }while(availableNodes.size()!=0);

    return false;
}

void game::Dijkstra_addNode(int index, std::shared_ptr<node> pre, double old_dis){
    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
        node myxNode(pos,pre);
        if(std::isinf(pos->getValue()) == false){
            double distance = old_dis + 1 + weight*(1-pos->getValue()); //pos->getValue() varies from 0 to 1
            myxNode.setDistance(distance);
        }//else distance is infinity by default
        availableNodes.push_back(myxNode); //qDebug()<<"Add new node with index:"<<index;
        myIndexes.insert(index);
    }
}

bool game::Dijkstra(int x, int y)
{

    do{
        node myNode = getNodeWithMinDistance();//get from SPT
        auto pre = std::make_shared<node>(myNode);
        double distance = myNode.getDistance();
        sptNodes.push_back(myNode);
        auto myTile = myNode.getTile();
        if((myTile->getXPos()== x) && (myTile->getYPos()== y))
            return true; //stop loop if found only in sptSet!
        if(myTile->getXPos()<xmax){
            int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() + 1;
            Dijkstra_addNode(index,pre, distance);
        }
        if(myTile->getXPos()>0){
            int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() - 1;
            Dijkstra_addNode(index,pre, distance);
        }
        if(myTile->getYPos()<ymax){
            int index = (myTile->getYPos() +1)*(xmax+1) + myTile->getXPos();
            Dijkstra_addNode(index,pre, distance);
        }
        if(myTile->getYPos()>0){
            int index = (myTile->getYPos() -1)*(xmax+1) + myTile->getXPos();
            Dijkstra_addNode(index,pre, distance);
        }
        //qDebug()<<"AvailableNodes Size:"<<availableNodes.size();
    }while(availableNodes.size()!=0);
    return false;//Not found if code goes outside of loop;
}

std::shared_ptr<EnemyUnit> game::getClosestEnemy(){
    float min_cost = std::numeric_limits<float>::infinity();
    auto result = defeatableEnemies[0];
    for(auto unit:defeatableEnemies){
        setDestination(unit->getXPos(),unit->getYPos());
        bool finished  = calcPath_Dijkstra();
        if(finished){  //path found, enemy is reachable
            float my_cost = getMoveCost();
            if(my_cost<min_cost){
                min_cost = my_cost;
                result = unit;
            }
            //clear memory created by finding the closet enemy
            setStart(protagonist->getXPos(),protagonist->getYPos());
            setMoveCost(0.0f);
        }
        screen->clearPath();
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
        bool finished  = calcPath_Dijkstra();
        if(finished){  //path found, health is reachable
            float my_cost = getMoveCost();
            if(my_cost<min_cost){
                min_cost = my_cost;
                result = pack;
            }
            //clear memory created by finding the closet enemy
            setStart(protagonist->getXPos(),protagonist->getYPos());
            setMoveCost(0.0f);
        }
        screen->clearPath();
    }

    //healthpacks.erase(result);
    return result;
}

float game::getMoveCost() const
{
    return moveCost;
}

void game::setMoveCost(float value)
{
    moveCost = value;
}

node game::getNodeWithMinDistance(){
    //find the node closted to des
    //remove the node from queue
    //return that node
    std::vector<node>::iterator result;
    result = std::min_element(availableNodes.begin(), availableNodes.end(), node_compare);
    node NodeWithMinDistance =*result;
    availableNodes.erase(result);
    return NodeWithMinDistance;
}

void game::clearLists()
{
    currentNodes.clear();
    availableNodes.clear();
    route.clear();
    myIndexes.clear();
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

int game::getWeight() const
{
    return weight;
}

void game::setWeight(int value)
{
    weight = value;
}

void game::setView(std::shared_ptr<view> test)
{
    screen = test;
}

std::shared_ptr<Tile> game::getTile(int x, int y)
{
    int index = xmax*x+y;
    auto tile = std::make_shared<Tile>(std::move(*tiles[index]));
    return tile;

}

void game::setDestination(int x, int y){
    xDest = x;
    yDest = y;
    emit destinationChanged(x,y);

}

bool game::calcPath_BreadthFirst(){

    /* as long as algorithm didn't reach destination and there are still nodes print path not found yet*/
    while((!breadthFirst(xDest,yDest)) &&(currentNodes.size())){
        //currentNodes.dequeue();
        qDebug()<< "Path not found yet!"<<" Queue Size = "<<currentNodes.size();
    }

    /* if there are no nodes left -> a path is found
       reverse through build link and determine path for animation*/
    if(currentNodes.size()){
        //qDebug()<< "Path found !!!!!";
        node destination = currentNodes.head();
        for(;destination.getPre()!=nullptr;){
            moveCost += 1+destination.getTile()->getValue();
            route.push(destination.getTile());
            screen->addPathStep(destination.getTile()->getXPos(),destination.getTile()->getYPos());
            destination=*(destination.getPre());
            //qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();


        }
    }else{
        qDebug()<< "Path is not found in the end!!!!!";
        return false;
    }
    return true;
}

bool game::calcPath_BestFirst()
{
    if(bestFirst(xDest,yDest)){
        //qDebug()<< "Path found !!!!!";
        node destination = availableNodes[availableNodes.size()-1];
        for(;destination.getPre()!=nullptr;){
            moveCost += 1+destination.getTile()->getValue();
            route.push(destination.getTile());
            screen->addPathStep(destination.getTile()->getXPos(),destination.getTile()->getYPos());
            destination=*(destination.getPre());
            //qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();
        }
        return true;
    }else{
        qDebug()<< "BestFirst:Path is not found in the end!!!!!";
        return false;
    }
}

bool game::calcPath_Dijkstra()
{
    QElapsedTimer timer;
    timer.start();

    if(Dijkstra(xDest,yDest)){
        //qDebug()<< "Path found !!!!!";
        node destination = sptNodes[sptNodes.size()-1];
        for(;destination.getPre()!=nullptr;){
            moveCost += 1+destination.getTile()->getValue();
            route.push(destination.getTile());
            screen->addPathStep(destination.getTile()->getXPos(),destination.getTile()->getYPos());
            destination=*(destination.getPre());
            //qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();
        }

//        qDebug() << "The run_strategy operation took" << timer.elapsed() << "milliseconds";
        return true;
    }else{
        qDebug()<< "AStar:Path is not found in the end!!!!!";
        clearLists();
        return false;
    }

}

void game::loadWorld(QString path){
    //clear enemy and healthpack vectors
    clearLists();
    enemies.clear();
    healthpacks.clear();

    //load image and determine size
    QImage image(path);
    xmax = image.width()-1;
    ymax = image.height()-1;

    //modify number of enemies and healthpacks based on world size. If world is huge, don't generate objects
    int objectNum = (xmax+ymax)/2;
    if(((xmax+ymax)/2)>100){
        objectNum = 1;
    }

    //Convert enemies to custom EnemyUnit
    tiles = myWorld->createWorld(path);
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
//    auto tempagonist = myWorld->getProtagonist();
    protagonist = std::shared_ptr<Hero>(new Hero());

    // Display the world into a graphicsscene
    screen->displayWorld(image);

}

void game::setStart(int x, int y){
    protagonist->setXPos(x);
    protagonist->setYPos(y);

    clearLists();
    auto pos1 = std::make_shared<Tile>(std::move(*(protagonist)));
    myIndexes.insert((xmax+1)*y+x);
    node myNode1(pos1,nullptr); //Starting point
    currentNodes.enqueue(myNode1);
    myNode1.setDistance(0);
    availableNodes.push_back(myNode1);
}

void game::MoveProtagonist()
{
    while(route.size()){
        auto tile = route.pop();
        protagonist->setXPos((tile->getXPos()));
        protagonist->setYPos((tile->getYPos()));
        float newEnergy = protagonist->getEnergy()-1 - getWeight()*(1-tile->getValue());
        protagonist->updateEnergy(newEnergy);
    }
    clearLists();
    setMoveCost(0.0f);
    setStart(xDest,yDest);
}

void game::checkMove(int xPos, int yPos){
    if(xPos>=0 && xPos<=xmax && yPos>=0 && yPos<=ymax){
        int index = yPos*(xmax+1) + xPos;
        if(!(std::isinf(tiles[index]->getValue()))){
            protagonist->setPos(xPos,yPos);
        }
    }

}

float game::getEnergy()
{
    return protagonist->getEnergy();
}

float game::getHealth()
{
    return protagonist->getHealth();
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
        finished = calcPath_BreadthFirst();
        break;
    case 4:
        finished = calcPath_BestFirst();
        break;
    default:
        finished = calcPath_Dijkstra();
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
    setWeight(weight);
    bool find = calcPath_Dijkstra();
    if(find){
        float requiredEnergy = getMoveCost();
        if(requiredEnergy>getEnergy()){
            return false; //quit the loop
        }else{
            MoveProtagonist();
            float newHealth = getHealth()+5.0*healthpack->getValue(); //multiply by a factor of 5
            if(newHealth > 100) newHealth = 100;
            protagonist->updateHealth(newHealth);
            setMoveCost(0.0f);
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
            if(unit->getValue() < getHealth()){ //when a enemy has a strength smaller than pro's health
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
    while(unit->getValue()>getHealth()){
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
    finished = calcPath_Dijkstra();
    if(finished){  //Path found
        float requiredEnergy = getMoveCost();
        if(requiredEnergy>getEnergy()){
            qDebug()<<"Game failed! Not enough energy to next enemy!Energy required: "<<requiredEnergy;
            setStart(protagonist->getXPos(),protagonist->getYPos());
            return false; //quit the loop
        }else{
            // Move the protagonist based on the calculated path
            MoveProtagonist();
            unit->kill();
            float newHealth = getHealth()-unit->getValue();
            protagonist->updateHealth(newHealth);
            protagonist->updateEnergy(100);
            setMoveCost(0.0f);
        }

        return true;
    }else{  //Path not found
        setStart(protagonist->getXPos(),protagonist->getYPos());
        return false;
    }
}


