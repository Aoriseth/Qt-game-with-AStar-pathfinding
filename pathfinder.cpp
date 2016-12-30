#include "pathfinder.h"

pathfinder::pathfinder()
{

}

int pathfinder::getWeight() const
{
    return weight;
}

void pathfinder::setWeight(int value)
{
    weight = value;
}

int pathfinder::getYmax() const
{
    return ymax;
}

void pathfinder::setYmax(int value)
{
    ymax = value;
}

int pathfinder::getXmax() const
{
    return xmax;
}

void pathfinder::setXmax(int value)
{
    xmax = value;
}

float pathfinder::getMoveCost() const
{
    return moveCost;
}

void pathfinder::setMoveCost(float value)
{
    moveCost = value;
}

void pathfinder::setXDest(int value)
{
    xDest = value;
}

void pathfinder::setYDest(int value)
{
    yDest = value;
}

int pathfinder::getXDest() const
{
    return xDest;
}

int pathfinder::getYDest() const
{
    return yDest;
}

std::shared_ptr<Tile> pathfinder::getTile(int x, int y)
{
    int index = xmax*x+y;
    auto tile = std::make_shared<Tile>(std::move(*tiles[index]));
    return tile;

}



void pathfinder::breadth_addNode(int index, std::shared_ptr<node> pre){
    if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
        if(!(std::isinf(tiles[index]->getValue()))){
            node myxNode(pos,pre);
            currentNodes.enqueue(myxNode);
            myIndexes.insert(index);
        }
    }
}

bool pathfinder::breadthFirst(int x,int y){
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

bool pathfinder::best_addNode(int x, int y, int index, std::shared_ptr<node> pre){
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
node pathfinder::getNodeWithMinDistance(){
    //find the node closted to des
    //remove the node from queue
    //return that node
    std::vector<node>::iterator result;
    result = std::min_element(availableNodes.begin(), availableNodes.end(), node_compare);
    node NodeWithMinDistance =*result;
    availableNodes.erase(result);
    return NodeWithMinDistance;
}
bool pathfinder::bestFirst(int x, int y)
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

void pathfinder::Dijkstra_addNode(int index, std::shared_ptr<node> pre, double old_dis){
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

bool pathfinder::Dijkstra(int x, int y)
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

bool pathfinder::calcPath_BreadthFirst(){

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

bool pathfinder::calcPath_BestFirst()
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

bool pathfinder::calcPath_Dijkstra()
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

void pathfinder::clearLists()
{
    currentNodes.clear();
    availableNodes.clear();
    route.clear();
    myIndexes.clear();
}
