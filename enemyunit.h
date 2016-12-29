#ifndef ENEMYUNIT_H
#define ENEMYUNIT_H
#include "world.h"
#include "enemyview.h"
#include <QObject>

class EnemyUnit: public QObject, public Enemy
{
    Q_OBJECT
public:
    EnemyUnit(int xPosition, int yPosition, float strength);
    void kill() {setDefeated(true);emit defeat();}
signals:
    void defeat();
};

#endif // ENEMYUNIT_H
