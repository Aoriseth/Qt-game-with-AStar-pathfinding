#ifndef ENEMYUNIT_H
#define ENEMYUNIT_H
#include "world.h"
#include "enemyview.h"
#include <QObject>
#include <QTimer>

class EnemyUnit: public QObject, public Enemy
{
    Q_OBJECT
public:
    EnemyUnit(int xPosition, int yPosition, float strength);
    void kill();

public slots:
    bool poison();

signals:
    void defeat(bool poisoned);
    void poisoning();

private:
    int poisonLeft = 3;
};

#endif // ENEMYUNIT_H
