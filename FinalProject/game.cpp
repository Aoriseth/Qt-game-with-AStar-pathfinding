#include "game.h"

game::game()
{
    ymax = 0;
    xmax= 0;
}

bool game::breadthFirst(int x,int y){
    QListIterator<node> i(currentNodes);
    while(i.hasNext()) {
        const node myNode = i.next();
        auto tile = myNode.getTile();
        if((tile->getXPos()==x) && (tile->getYPos()==y)){
            return true;
        }else{

                if(tile->getXPos()<xmax){
                    int index = (xmax+1)*(tile->getYPos()) + tile->getXPos() + 1;
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }
                if(tile->getXPos()>0){
                    auto index = (xmax+1)*(tile->getYPos()) + tile->getXPos() - 1;
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }
                if(tile->getYPos()<ymax){
                    int index = (tile->getYPos() +1)*(xmax+1) + tile->getXPos();
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }
                if(tile->getYPos()>0){
                    int index = (tile->getYPos() -1)*(xmax+1) + tile->getXPos();
                    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        if(!(std::isinf(tiles[index]->getValue()))){
                            node myxNode(pos,pre);
                            currentNodes.enqueue(myxNode);
                            myIndexes.insert(index);
                        }
                    }
                }


            currentNodes.dequeue();
        }
    }
    return false;
}

bool game::bestFirst(int x, int y)
{
    do{
            node myNode = getNodeWithMinDistance();
            auto myTile = myNode.getTile();
            if(myTile->getXPos()<xmax){
                int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() + 1;
                if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                    if(!(std::isinf(tiles[index]->getValue()))){
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        node myxNode(pos,pre);
                        double distance =pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2);
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode); //qDebug()<<"x+:Add new node with index:"<<index;
                        myIndexes.insert(index);qDebug()<<"Distance:"<<distance;
                        if(distance==0)
                            return true;
                    }
                }
            }
            if(myTile->getXPos()>0){
                int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() - 1;
                if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                    if(!(std::isinf(tiles[index]->getValue()))){
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        node myxNode(pos,pre);
                        double distance =pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2);
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode);//qDebug()<<"x-:Add new node with index:"<<index;
                        myIndexes.insert(index);qDebug()<<"Distance:"<<distance;
                        if(distance==0)
                            return true;
                    }
                }
            }
            if(myTile->getYPos()<ymax){
                int index = (myTile->getYPos() +1)*(xmax+1) + myTile->getXPos();
                if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                    if(!(std::isinf(tiles[index]->getValue()))){
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        node myxNode(pos,pre);
                        double distance =pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2);
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode);//qDebug()<<"y+:Add new node with index"<<index;
                        myIndexes.insert(index);qDebug()<<"Distance:"<<distance;
                        if(distance==0)
                            return true;
                    }
                }
            }
            if(myTile->getYPos()>0){
                int index = (myTile->getYPos() -1)*(xmax+1) + myTile->getXPos();
                if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                    if(!(std::isinf(tiles[index]->getValue()))){
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        node myxNode(pos,pre);
                        double distance =pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2);
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode);//qDebug()<<"y-:Add new node with index"<<index;
                        myIndexes.insert(index);qDebug()<<"Distance:"<<distance;
                        if(distance==0)
                            return true;
                    }
                }
            }
            //qDebug()<<"Size:"<<availableNodes.size();
        }while(availableNodes.size()!=0);

    return false;
}

bool game::AStar(int x, int y)
{
    do{
        node myNode = getNodeWithMinDistance();//get from SPT
        sptNodes.push_back(myNode);
        auto myTile = myNode.getTile();
        if((myTile->getXPos()== x) && (myTile->getYPos()== y))
            return true; //stop loop if found only in sptSet!

        if(myTile->getXPos()<xmax){
            int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() + 1;
            if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                auto pre = std::make_shared<node>(myNode);
                auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                node myxNode(pos,pre);
                if(std::isinf(pos->getValue()) == false){
                    double distance = myNode.getDistance() + 1 + weight*(1-pos->getValue()); //pos->getValue() varies from 0 to 1
                    myxNode.setDistance(distance);
                }//else distance is infinity by default
                availableNodes.push_back(myxNode); //qDebug()<<"x+:Add new node with index:"<<index;
                myIndexes.insert(index);
            }
        }
        if(myTile->getXPos()>0){
            int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() - 1;
            if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                auto pre = std::make_shared<node>(myNode);
                auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                node myxNode(pos,pre);
                if(std::isinf(pos->getValue()) == false){
                    double distance = myNode.getDistance() + 1 + weight*(1-pos->getValue()); //pos->getValue() varies from 0 to 1
                    myxNode.setDistance(distance);
                }//else distance is infinity by default
                availableNodes.push_back(myxNode); //qDebug()<<"x+:Add new node with index:"<<index;
                myIndexes.insert(index);
            }
        }
        if(myTile->getYPos()<ymax){
            int index = (myTile->getYPos() +1)*(xmax+1) + myTile->getXPos();
            if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                auto pre = std::make_shared<node>(myNode);
                auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                node myxNode(pos,pre);
                if(std::isinf(pos->getValue()) == false){
                    double distance = myNode.getDistance() + 1 + weight*(1-pos->getValue()); //pos->getValue() varies from 0 to 1
                    myxNode.setDistance(distance);
                }//else distance is infinity by default
                availableNodes.push_back(myxNode); //qDebug()<<"x+:Add new node with index:"<<index;
                myIndexes.insert(index);
            }
        }
        if(myTile->getYPos()>0){
            int index = (myTile->getYPos() -1)*(xmax+1) + myTile->getXPos();
            if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                auto pre = std::make_shared<node>(myNode);
                auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                node myxNode(pos,pre);
                if(std::isinf(pos->getValue()) == false){
                    double distance = myNode.getDistance() + 1 + weight*(1-pos->getValue()); //pos->getValue() varies from 0 to 1
                    myxNode.setDistance(distance);
                }//else distance is infinity by default
                availableNodes.push_back(myxNode); //qDebug()<<"x+:Add new node with index:"<<index;
                myIndexes.insert(index);
            }
        }
        //qDebug()<<"AvailableNodes Size:"<<availableNodes.size();
    }while(availableNodes.size()!=0);
    return false;//Not found if code goes outside of loop;
}

std::vector<std::unique_ptr<Enemy>>::iterator game::getClosestEnemy(){
    float min_cost = 10000;
    auto result = enemies.begin();
    for(auto it = enemies.begin(); it != enemies.end(); ++it){
        if(!((*it)->getDefeated())){  //only check those undefeated enemies.
            setDestination((*it)->getXPos(),(*it)->getYPos());
            bool finished  = calcPath_AStar();
            if(finished){  //path found, enemy is reachable
                float my_cost = getMoveCost();
                if(my_cost<min_cost){
                    min_cost = my_cost;
                    result = it;
                }
                //clear memory created by finding the closet enemy
                setStart(protagonist->getXPos(),protagonist->getYPos());
                setMoveCost(0.0f);
            }
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

std::vector<std::unique_ptr<Tile>>::iterator game::getClosestHealthpack()
{
    float min_cost = 10000;
    auto result = healthpacks.begin();
    for(std::vector<std::unique_ptr<Tile>>::iterator it = healthpacks.begin(); it != healthpacks.end(); ++it){
        setDestination((*it)->getXPos(),(*it)->getYPos());
        bool finished  = calcPath_AStar();
        if(finished){  //path found, health is reachable
            float my_cost = getMoveCost();
            if(my_cost<min_cost){
                min_cost = my_cost;
                result = it;
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

    while(!isAllDefeated()){
        std::vector<std::shared_ptr<Enemy>> list;
        for(auto& unit:enemies){
            if ((unit->getValue()<protagonist->getHealth())&&(!unit->getDefeated())){
                list.push_back(std::move(unit));
            }
        }

        std::shared_ptr<Enemy> target;
        QStack<std::shared_ptr<Tile>> chosenRoute;
        float lowest = std::numeric_limits<float>::infinity();
        setWeight(1);

        for(auto unit:list){
            setDestination(unit->getXPos(),unit->getYPos());
            if(calcPath_AStar()){
                if(getMoveCost()<lowest){
                    lowest = getMoveCost();
                    chosenRoute = route;
                    target=unit;
                }
                screen->clearPath();

            }
            else{
                chosenRoute.clear();
            }
            setStart(protagonist->getXPos(),protagonist->getYPos());
            setMoveCost(0.0f);
        }
        route = chosenRoute;
        MoveProtagonist();
        target->setDefeated(true);

    }





}

int game::getWeight() const
{
    return weight;
}

void game::setWeight(int value)
{
    weight = value;
}

void game::setView(view* test)
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

}

bool game::calcPath_BreadthFirst(){

    // as long as algorithm didn't reach destination and there are still nodes print path not found yet
    while((!breadthFirst(xDest,yDest)) &&(currentNodes.size())){
        //currentNodes.dequeue();
        qDebug()<< "Path not found yet!"<<" Queue Size = "<<currentNodes.size();
    }

    // if there are no nodes left -> a path is found
    // reverse through build link and determine path for animation
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

bool game::calcPath_AStar()
{

    if(AStar(xDest,yDest)){
        //qDebug()<< "Path found !!!!!";
        node destination = sptNodes[sptNodes.size()-1];
        for(;destination.getPre()!=nullptr;){
            moveCost += 1+destination.getTile()->getValue();
            route.push(destination.getTile());
            screen->addPathStep(destination.getTile()->getXPos(),destination.getTile()->getYPos());
            destination=*(destination.getPre());
            //qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();
        }
        return true;
    }else{
        qDebug()<< "Dijkstra:Path is not found in the end!!!!!";
        return false;
    }
}

void game::loadWorld(QString path, QGraphicsScene * scene){
    tiles = myWorld->createWorld(path);
    enemies = myWorld->getEnemies(5);

    healthpacks = myWorld->getHealthPacks(5);
    protagonist = myWorld->getProtagonist();
    setHealth(100);
    setEnergy(100);

    xmax=0;ymax=0;


    QImage image(path);
    worldView = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    xmax = image.width()-1;
    ymax = image.height()-1;
    worldView->setZValue(0);
    worldView->setScale(1);
    worldView->setPos(getProtagonistX(),getProtagonistY());
    screen->sceneView->addItem(worldView);
    qDebug()<<"ymax is "<<ymax;
    qDebug()<<"xmax is "<<xmax;
//    for(auto& unit:enemies){
//        int index = (unit->getYPos())*(xmax+1) + unit->getXPos();
//        tiles[index]->setValue(std::numeric_limits<float>::infinity());
//        qDebug()<< "index: "<<index;
//   }
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
        protagonist->setEnergy(newEnergy);
        emit changeStats(protagonist->getEnergy(),protagonist->getHealth());

    }
}

int game::getProtagonistX()
{
    return protagonist->getXPos();
}

int game::getProtagonistY()
{
    return protagonist->getYPos();
}

void game::setEnergy(float passEnergy)
{
    protagonist->setEnergy(passEnergy);
    emit changeStats(protagonist->getEnergy(), protagonist->getHealth());
}

void game::setHealth(float passHealth)
{
    protagonist->setHealth(passHealth);
    emit changeStats(protagonist->getEnergy(), protagonist->getHealth());

}

float game::getEnergy()
{
    return protagonist->getEnergy();
}

float game::getHealth()
{
    return protagonist->getHealth();
}

Protagonist* game::getProtagonist()
{
    return protagonist.get();
}

void game::removeHealthpack(std::unique_ptr<Tile>& healthpack)
{
    int pos = find(healthpacks.begin(), healthpacks.end(), healthpack) - healthpacks.begin();
    emit healthpackUsed(pos);
}

bool game::goForHealthpack()
{
    std::vector<std::unique_ptr<Tile>>::iterator healthpack = getClosestHealthpack();
    setDestination((*healthpack)->getXPos(),(*healthpack)->getYPos());
    setWeight(weight);
    bool find = calcPath_AStar();
    if(find){
        float requiredEnergy = getMoveCost();
        if(requiredEnergy>getEnergy()){
            qDebug()<<"Game failed! Not enough energy to closest healthpack!Energy required: "<<requiredEnergy;
            return false; //quit the loop
        }else{
            float newHealth = getHealth()+5.0*(*healthpack)->getValue(); //multiply by a factor of 5
            if(newHealth > 100) newHealth = 100;
            setHealth(newHealth);
            setMoveCost(0.0f);
            // Move the protagonist based on the calculated path
            MoveProtagonist();
            qDebug()<<"Succeed to get a healthpack!";
            removeHealthpack(*healthpack);
            qDebug()<<"New Health is "<<getHealth();
            setStart(xDest,yDest);
            return true;
        }

    }else{
        qDebug()<<"Healthpack is not found!";
        return false;
    }
}

bool game::isDefeatable()
{
    for(auto it = enemies.begin(); it != enemies.end(); ++it){
        if(!(*it)->getDefeated()){  //when a enemy is not defeated
            if((*it)->getValue() < getHealth()){ //when a enemy has a strength smaller than pro's health
                //qDebug()<<"At least one enemy is defeatable with strength: "<<(*it)->getValue();
                return true;
            }
        }
    } //qDebug()<<"NO enemy can be defeated!";
    return false;
}

bool game::goForEnemy()
{
    auto it = getClosestEnemy();  //calculate the path and get the closest enemy
    Enemy closestEnemy = **it;
    //After the check that there is at least one defeatable enemy, which reduce the calls of getClosestEnemy();
    //the closestEnemy could still be undefeatable
    while(closestEnemy.getValue()>getHealth()){
        qDebug()<<"Health is not enough to defeat an enemy, go for healthpack";
        if(healthpacks.size()==0){
            qDebug()<<"Quit: NO healthpack left!";
            return false;
        }
        if(!goForHealthpack()){
            return false; //quite the loop
        }
    }
    setDestination(closestEnemy.getXPos(),closestEnemy.getYPos());
    bool finished = false;
    finished = calcPath_AStar();
    if(finished){  //Path found
        float requiredEnergy = getMoveCost();
        if(requiredEnergy>getEnergy()){
            qDebug()<<"Game failed! Not enough energy to next enemy!Energy required: "<<requiredEnergy;
            return false; //quit the loop
        }else{
            float newHealth = getHealth()-closestEnemy.getValue();
            setHealth(newHealth);
            setEnergy(100);
            setMoveCost(0.0f);
        }
        // Move the protagonist based on the calculated path
        MoveProtagonist();
        killEnemy(*it);
        qDebug()<<"Succeed to kill an enemy!";
        qDebug()<<"New health is "<<getHealth();
        setStart(xDest,yDest);
        return true;
    }else{  //Path not found
        setStart(getProtagonistX(),getProtagonistY());
        return false;
    }
}

void game::killEnemy(std::unique_ptr<Enemy>& destroyee)
{
    destroyee->setDefeated(true); //mark this enemy as defeated
    int pos = find(enemies.begin(), enemies.end(), destroyee) - enemies.begin();
    emit enemyKilled(pos);
}

