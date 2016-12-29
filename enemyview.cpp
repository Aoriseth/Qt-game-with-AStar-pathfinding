#include "enemyview.h"

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

    text = std::make_shared<QGraphicsTextItem>();
    text->setScale(0.00390625*2);
    text->setZValue(2);
    text->setPos(mEnemy->getXPos(),mEnemy->getYPos());
    text->setPlainText(QString::number(mEnemy->getValue()));
    view->addItem(text.get());
}

void EnemyView::updateVisual()
{
    text.reset();
    this->setPixmap(QPixmap(":/resources/goomba_dead.gif"));
}
