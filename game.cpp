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
            node myNode = getTheClosestNode();
            auto myTile = myNode.getTile();
            if(myTile->getXPos()<xmax){
                int index = (xmax+1)*(myTile->getYPos()) + myTile->getXPos() + 1;
                if(!myIndexes.contains(index)) {/* myIndexes doesn't contain index */
                    if(!(std::isinf(tiles[index]->getValue()))){
                        auto pre = std::make_shared<node>(myNode);
                        auto pos = std::make_shared<Tile>(std::move(*tiles[index]));
                        node myxNode(pos,pre);
                        int distance =(int)(pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2));
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode); qDebug()<<"x+:Add new node with index:"<<index;
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
                        int distance =(int)(pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2));
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode);qDebug()<<"x-:Add new node with index:"<<index;
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
                        int distance =(int)(pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2));
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode);qDebug()<<"y+:Add new node with index"<<index;
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
                        int distance =(int)(pow(pos->getXPos()-x,2)+pow(pos->getYPos()-y,2));
                        myxNode.setDistance(distance);
                        availableNodes.push_back(myxNode);qDebug()<<"y-:Add new node with index"<<index;
                        myIndexes.insert(index);qDebug()<<"Distance:"<<distance;
                        if(distance==0)
                            return true;
                    }
                }
            }
            qDebug()<<"Size:"<<availableNodes.size();
        }while(availableNodes.size()!=0);

        return false;
}



node game::getTheClosestNode(){
    //find the node closted to des
    //remove the node from queue
    //return that node
    std::vector<node>::iterator result;
    result = std::min_element(availableNodes.begin(), availableNodes.end(), abs_compare);
    node closestNode =*result;
    availableNodes.erase(result);
    return closestNode;
}

void game::clearLists()
{
    currentNodes.clear();
    availableNodes.clear();
    route.clear();
    myIndexes.clear();
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
        qDebug()<< "Path found !!!!!";
        node destination = currentNodes.head();
        for(;destination.getPre()!=nullptr;){
            route.push(destination.getTile());
            destination=*(destination.getPre());
            qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();


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
        qDebug()<< "Path found !!!!!";
        node destination = availableNodes[availableNodes.size()-1];
        for(;destination.getPre()!=nullptr;){
            route.push(destination.getTile());
            destination=*(destination.getPre());
            qDebug()<< "X: "<<destination.getTile()->getXPos()<<"Y: "<<destination.getTile()->getYPos();
        }
        return true;
    }else{
        qDebug()<< "BestFirst:Path is not found in the end!!!!!";
        return false;
    }
}

void game::loadWorld(QString path, QGraphicsScene * scene){
    tiles = myWorld->createWorld(path);
    enemies = myWorld->getEnemies(2);
    healthpacks = myWorld->getHealthPacks(2);
    protagonist = myWorld->getProtagonist();

    xmax=0;ymax=0;
    for(auto& tile: tiles){
        int x = tile->getXPos();
        if (x>xmax){
            xmax=x;
        }
        int y = tile->getYPos();
        if (y>ymax){
            ymax=y;

        }
        float value = tile->getValue();
        if(std::isinf(value)) value= 255.0;
        else value = 255.0-value*255.0;
        scene->addRect(256*x, 256*y, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 0,(int)value)));
    }
    qDebug()<<"ymax is "<<ymax;
    qDebug()<<"xmax is "<<xmax;
}

void game::setStart(int x, int y){
    protagonist->setXPos(x);
    protagonist->setYPos(y);

    clearLists();
    auto pos1 = std::make_shared<Tile>(std::move(*(protagonist)));
    myIndexes.insert((xmax+1)*y+x);
    node myNode1(pos1,nullptr); //Starting point
    currentNodes.enqueue(myNode1);
    availableNodes.push_back(myNode1);
}
