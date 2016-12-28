#include "healthmodel.h"
#include <QDebug>

HealthModel::HealthModel(int xPosition, int yPosition, float tileWeight):
    Tile(xPosition, yPosition, tileWeight)
{

}


void HealthModel::useHealthpack()
{
    emit used();
}
