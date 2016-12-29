#include "hero.h"

Hero::Hero()
{
    updateHealth(100.0);
    updateEnergy(100.0);
    setXPos(0);
    setYPos(0);
}

void Hero::updateEnergy(float value)
{
    setEnergy(value);
    emit changeStats(getEnergy(),getHealth());
}

void Hero::updateHealth(float value)
{
    setHealth(value);
    emit changeStats(getEnergy(),getHealth());

}

void Hero::MoveProLeft(){
    auto x = getXPos();
    emit moveIt(--x,getYPos());
}

void Hero::MoveProRight(){
    auto x = getXPos();
    emit moveIt(++x,getYPos());
}

void Hero::MoveProUp(){
    auto y = getYPos();
    emit moveIt(getXPos(),--y);
}

void Hero::MoveProDown(){
    auto y = getYPos();
    emit moveIt(getXPos(),++y);
}

void Hero::movePro(int x)
{
    switch(x){
    case 1:
        MoveProLeft();
        break;
    case 2:
        MoveProRight();
        break;
    case 3:
        MoveProUp();
        break;
    case 4:
        MoveProDown();
        break;
    default:
        break;
    }
}

