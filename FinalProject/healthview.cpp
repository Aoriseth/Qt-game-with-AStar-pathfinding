#include "healthview.h"

HealthView::HealthView()
{

}

HealthView::HealthView(QPixmap pm, Scene *vPass, std::shared_ptr<HealthModel> mPass):
    QGraphicsPixmapItem(pm)
{
    view = vPass;
    mHealth = mPass;
}

void HealthView::addToScene(){
    this->setScale(0.00390625);
    this->setPos(mHealth->getXPos(),mHealth->getYPos());
    this->setZValue(2);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    view->addItem(this);

    QGraphicsTextItem * io = new QGraphicsTextItem;
    io->setScale(0.00390625*2);
    io->setZValue(2);
    io->setPos(mHealth->getXPos(),mHealth->getYPos());
    io->setPlainText(QString::number(mHealth->getValue()));
    view->addItem(io);
}

void HealthView::healthUsed()
{
    view->removeItem(this);
    delete this;
}
