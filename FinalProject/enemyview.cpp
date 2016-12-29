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

void EnemyView::updateVisual()
{
    // update the view to a dead enemy
    text.reset();
    this->setPixmap(QPixmap(":/resources/goomba_dead.gif"));
}
