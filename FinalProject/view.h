#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include "game.h"
#include "scene.h"
#include "enemyview.h"
#include <QObject>

class game;

class view: public QObject
{
    Q_OBJECT
public:
    view();
    std::shared_ptr<Scene> sceneView;
    std::shared_ptr<game> logic;
    std::shared_ptr<QGraphicsItem> destView;
    std::vector<std::shared_ptr<QGraphicsItem>> pathView;
    void addPathStep(int x,int y);
    void clearPath();
    void setLogic(std::shared_ptr<game> pass);
    void showEnemies();
    void showHealthpacks();
    void showProtagonist();
    void clearLists();
    void initDestination();
    void setProtagonistPosition(int x, int y);
    std::shared_ptr<QGraphicsPixmapItem> getWorldView() const;
    void displayWorld(QImage image);

private:
    std::shared_ptr<QGraphicsPixmapItem> worldView;
    std::shared_ptr<QGraphicsPixmapItem> protagonistView;
    std::vector<EnemyView*> enemyItems;
    std::vector<QGraphicsPixmapItem*> healthpackItems;
signals:
    void updateViewport();
protected slots:
    void indicateDestination(int x, int y);

};

#endif // VIEW_H
