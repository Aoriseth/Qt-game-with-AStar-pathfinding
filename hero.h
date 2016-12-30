#ifndef HERO_H
#define HERO_H
#include "world.h"
#include <QDebug>

class Hero: public Protagonist
{
    Q_OBJECT
public:
    Hero();
    void updateEnergy(float value);
    void updateHealth(float value);

private:
    void MoveProLeft();
    void MoveProRight();
    void MoveProUp();
    void MoveProDown();

signals:
    void changeStats(float energy, float health);
    void moveIt(int x, int y);

protected slots:
    void movePro(int x);
};

#endif // HERO_H
