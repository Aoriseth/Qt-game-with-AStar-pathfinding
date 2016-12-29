#include "enemyview.h"

EnemyView::EnemyView()
{

}

EnemyView::EnemyView(QPixmap pm, std::shared_ptr<Scene> vPass, std::shared_ptr<EnemyUnit> mPass):
    QGraphicsPixmapItem(pm)
{
    view = vPass;
    mEnemy = mPass;

}

void EnemyView::addToScene(){
    this->setScale(0.00390625);
    this->setPos(mEnemy->getXPos(),mEnemy->getYPos());
    this->setZValue(2);
    this->setFlag(QGraphicsItem::ItemIsSelectable, true);
    view->addItem(this);

    QGraphicsTextItem * io = new QGraphicsTextItem;
    io->setScale(0.00390625*2);
    io->setZValue(2);
    io->setPos(mEnemy->getXPos(),mEnemy->getYPos());
    io->setPlainText(QString::number(mEnemy->getValue()));
    view->addItem(io);
}

void EnemyView::updateVisual()
{
    this->setPixmap(QPixmap(":/resources/goomba_dead.gif"));
}
