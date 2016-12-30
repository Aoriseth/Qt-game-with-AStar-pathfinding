#include "enemyunit.h"

EnemyUnit::EnemyUnit(int xPosition, int yPosition, float strength):
    Enemy(xPosition, yPosition, strength)
{

}

void EnemyUnit::kill()
{
     setDefeated(true);
     auto chance = std::rand() % 10;
     if(chance>1){
         emit defeat(false);
     }else{
         emit defeat(true);
         QTimer::singleShot(1000, this, SLOT(poison()));
     }
}

bool EnemyUnit::poison()
{
    if(poisonLeft){
        QTimer::singleShot(1000, this, SLOT(poison()));
        poisonLeft--;
        emit poisoning();

    }else{
        emit defeat(false);
    }

}
