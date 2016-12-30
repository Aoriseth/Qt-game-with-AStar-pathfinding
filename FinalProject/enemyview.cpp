#include "enemyview.h"

EnemyView::EnemyView(QPixmap pm, std::shared_ptr<Scene> vPass, std::shared_ptr<EnemyUnit> mPass):
    QGraphicsPixmapItem(pm)
{
    view = vPass;
    mEnemy = mPass;

}

void EnemyView::addToScene(){

    // add this item to the scene
    this->setScale(0.00390625);
    this->setPos(mEnemy->getXPos(),mEnemy->getYPos());
    this->setZValue(2);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    view->addItem(this);

    // add text to the scene to display strength
    text = std::make_shared<QGraphicsTextItem>();
    text->setScale(0.00390625*2);
    text->setZValue(2);
    text->setPos(mEnemy->getXPos(),mEnemy->getYPos());
    text->setPlainText(QString::number(mEnemy->getValue()));
    view->addItem(text.get());
}

void EnemyView::updateVisual(bool poisoned)
{
    // update the view to a dead enemy
    text.reset();
    if(poisoned){
        this->setPixmap(QPixmap(":/resources/goombaPoisoned.gif"));
    }else{
        this->setPixmap(QPixmap(":/resources/goomba_dead.gif"));
    }
}

void EnemyView::affectArea()
{
    this->setPixmap(QPixmap(":/resources/goombaPoisoned_intermediate.gif"));
    for(auto x=-1;x<=1;x++){
        for(auto y=-1;y<=1;y++){
            view->addRect(mEnemy->getXPos()+x, mEnemy->getYPos()+y, 1, 1, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 255, 0,50)));
        }
    }
    qDebug()<<"Poisoning";
}
