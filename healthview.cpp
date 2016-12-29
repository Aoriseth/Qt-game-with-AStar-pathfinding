#include "healthview.h"

HealthView::HealthView(QPixmap pm, std::shared_ptr<Scene>vPass, std::shared_ptr<HealthModel> mPass):
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

    text = std::make_shared<QGraphicsTextItem>();
    text->setScale(0.00390625*2);
    text->setZValue(2);
    text->setPos(mHealth->getXPos(),mHealth->getYPos());
    text->setPlainText(QString::number(mHealth->getValue()));
    view->addItem(text.get());
}

void HealthView::healthUsed()
{
    view->removeItem(this);
    delete this;
}
