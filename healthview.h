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
    HealthView(QPixmap pm, std::shared_ptr<Scene>vPass, std::shared_ptr<HealthModel> mPass);
    void addToScene();
private:
    std::shared_ptr<Scene>view;
    std::shared_ptr<HealthModel> mHealth;
    std::shared_ptr<QGraphicsTextItem> text;
protected slots:
    void healthUsed();
};

#endif // HEALTHVIEW_H
