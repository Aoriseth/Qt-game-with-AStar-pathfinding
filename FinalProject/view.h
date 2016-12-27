#ifndef VIEW_H
#define VIEW_H

#include <QGraphicsScene>
#include <QGraphicsPixmapItem>


class view
{
public:
    view();
    QGraphicsScene * scene;

    void addItemToScene(QGraphicsPixmapItem *item, int x, int y);
};

#endif // VIEW_H
