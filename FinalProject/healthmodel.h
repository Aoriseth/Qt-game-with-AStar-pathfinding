#ifndef HEALTHMODEL_H
#define HEALTHMODEL_H
#include "world.h"
#include "healthview.h"

class HealthView;
#include <QObject>


class HealthModel:public QObject, public Tile
{
    Q_OBJECT
public:
    HealthModel(int xPosition, int yPosition, float tileWeight);
    void useHealthpack();

signals:
    void used();
};

#endif // HEALTHMODEL_H
