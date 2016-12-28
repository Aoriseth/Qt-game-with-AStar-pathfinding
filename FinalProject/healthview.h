#ifndef HEALTHVIEW_H
#define HEALTHVIEW_H
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <scene.h>
#include "healthmodel.h"
#include <QObject>

class HealthModel;

class HealthView: public QObject,public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    HealthView();
    HealthView(QPixmap pm, Scene *vPass, std::shared_ptr<HealthModel> mPass);
    void addToScene();
private:
    Scene *view;
    std::shared_ptr<HealthModel> mHealth;
protected slots:
    void healthUsed();
};

#endif // HEALTHVIEW_H
