#include "mainwindow.h"
#include "ui_mainwindow.h"



QGraphicsItem * destView;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::play_clicked()
{

    if(!mapLoaded){
        return;
    }

    auto finished = false;

    // Check the chosen algorithm
    switch (ui->comboBox->currentIndex()) {
    case 0:
        logic->setWeight(10);
        finished = logic->calcPath_Dijkstra();
        break;
    case 1:
        finished = logic->calcPath_BreadthFirst();
        break;
    case 2:
        finished = logic->calcPath_BestFirst();
        break;
    default:
        break;
    }

    // Move the protagonist based on the calculated path
    if(finished){
        while(logic->route.size()){
            auto tile = logic->route.pop();
            protagonistView->setPos(256*(tile->getXPos()),256*(tile->getYPos()));
            ui->graphicsView->viewport()->repaint();
            float newEnergy = logic->protagonist->getEnergy()-1 - logic->getWeight()*(1-tile->getValue());
            logic->protagonist->setEnergy(newEnergy);
            updateStats();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        logic->setStart(logic->xDest,logic->yDest);
    }else{
        logic->setStart(logic->protagonist->getXPos(),logic->protagonist->getYPos());
    }
}


void MainWindow::execute_strategy()
{
    logic->protagonist->setEnergy(100);
    while(!logic->isAllDefeated()){ //check if all enemies are deafeated
        auto finished = false;
        auto it = logic->getClosestEnemy();
        Enemy closestEnemy = **it;
        if((*it)->getValue()>logic->protagonist->getHealth()){
            qDebug()<<"Health is not enough to defeat an enemy, go for healthpack";
            if(logic->healthpacks.size()==0){
                qDebug()<<"Quit: NO healthpack left!";
                return;
            }
            Tile healthpack = logic->getClosestHealthpack();
            logic->setDestination(closestEnemy.getXPos(),closestEnemy.getYPos());
            logic->setWeight(5);
            bool find = logic->calcPath_Dijkstra();
            if(find){
                float requiredEnergy = logic->getMoveCost();
                if(requiredEnergy>logic->protagonist->getEnergy()){
                    qDebug()<<"Game failed! Not enough energy to closest healthpack!Energy required: "<<requiredEnergy;
                    return; //quit the loop
                }else{
                    float newHealth = logic->protagonist->getHealth()+10.0*healthpack.getValue();
                    if(newHealth > 100) newHealth = 100;
                    logic->protagonist->setHealth(newHealth);
                    logic->setMoveCost(0.0f);
                    // Move the protagonist based on the calculated path
                    while(logic->route.size()){qDebug()<<"Go to Healthpack!";
                        auto tile = logic->route.pop();
                        protagonistView->setPos(256*(tile->getXPos()),256*(tile->getYPos()));
                        ui->graphicsView->viewport()->repaint();
                        float newEnergy = logic->protagonist->getEnergy()-1 - logic->getWeight()*(1-tile->getValue());
                        logic->protagonist->setEnergy(newEnergy);
                        updateStats();
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                    qDebug()<<"Succeed to get a healthpack!";
                    qDebug()<<"New Health is "<<logic->protagonist->getHealth();
                    logic->setStart(logic->xDest,logic->yDest);
                    std::this_thread::sleep_for(std::chrono::milliseconds(100));
                    auto it = logic->getClosestEnemy();
                    closestEnemy = **it;
                    if((*it)->getValue()>logic->protagonist->getHealth()){
                        qDebug()<<"Strength of the enemy is " << (*it)->getValue();
                        qDebug()<<"Quit: Health is still not enough to defeat an enemy!!!";
                        return; //stop this strategy
                    }
                    qDebug()<<"Health is now enough to defeat an enemy!!!";
                }

            }
        }
        (*it)->setDefeated(true); //mark this enemy as defeated

        logic->setDestination(closestEnemy.getXPos(),closestEnemy.getYPos());
        logic->setWeight(5);
        finished = logic->calcPath_Dijkstra();
        if(finished){  //Path found
            float requiredEnergy = logic->getMoveCost();
            if(requiredEnergy>logic->protagonist->getEnergy()){
                qDebug()<<"Game failed! Not enough energy to next enemy!Energy required: "<<requiredEnergy;
                return; //quit the loop
            }else{
                float newHealth = logic->protagonist->getHealth()-closestEnemy.getValue();
                logic->protagonist->setHealth(newHealth);
                qDebug()<<"Health is "<<logic->protagonist->getHealth();
                logic->protagonist->setEnergy(100);
                logic->setMoveCost(0.0f);
            }
            // Move the protagonist based on the calculated path
            while(logic->route.size()){
                auto tile = logic->route.pop();
                protagonistView->setPos(256*(tile->getXPos()),256*(tile->getYPos()));
                ui->graphicsView->viewport()->repaint();
                float newEnergy = logic->protagonist->getEnergy()-1 - logic->getWeight()*(1-tile->getValue());
                logic->protagonist->setEnergy(newEnergy);
                updateStats();
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
            }
            qDebug()<<"Succeed to kill an enemy!";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            logic->setStart(logic->xDest,logic->yDest);
        }else{  //Path not found
            logic->setStart(logic->protagonist->getXPos(),logic->protagonist->getYPos());
        }
    }
    logic->setDestination(0,0);
    indicateDestination(0,0);
    qDebug()<<"Game Win! All the enemies are defeated!";

}

void MainWindow::refreshScene(){
    screen->scene = new QGraphicsScene(this);
    connect(screen->scene, SIGNAL(selectionChanged()), this, SLOT(ItemSelected()));
    ui->graphicsView->setScene(screen->scene);

    //scene->setSceneRect(scene->itemsBoundingRect());
}

void MainWindow::showProtagonist(){
    QImage image(":/resources/Pixel-mario.png");
    protagonistView = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    protagonistView->setZValue(3);
    protagonistView->setScale(1);
    protagonistView->setPos(logic->protagonist->getXPos(),logic->protagonist->getYPos());
    screen->scene->addItem(protagonistView);
}

void MainWindow::showHealthpacks(){
    for(auto& healthpack: logic->healthpacks){
        int x = healthpack->getXPos();
        int y = healthpack->getYPos();
        QImage image(":/resources/Supermushroom.png");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        screen->addItemToScene(item, x, y);
        healthpackItems.push_back(item);
    }
}

void MainWindow::showEnemies(){
    for(auto& enemy: logic->enemies){
        int x = enemy->getXPos();
        int y = enemy->getYPos();
        QImage image(":/resources/goomba.gif");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        screen->addItemToScene(item, x, y);
        enemyItems.push_back(item);
    }
}

void MainWindow::indicateDestination(int x, int y){
    destView->setPos(256*x,256*y);
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::setLogic(game *test)
{
    logic = test;
}

void MainWindow::setView(view *pass)
{
    screen = pass;
}

void MainWindow::updateStats(){
    ui->energyBar->setValue(logic->protagonist->getEnergy());
    ui->healthBar->setValue(logic->protagonist->getHealth());
    qDebug()<<"Energy is "<<logic->protagonist->getEnergy();

}


void MainWindow::OpenMap()
{
    mapLoaded = true;
    //clear logic lists and refresh the scene
    logic->clearLists();
    refreshScene();

    //set path chosen by user
    QString path = QFileDialog::getOpenFileName(this,tr("Select map"));

    //loads world into scene
    logic->loadWorld(path,screen->scene);

    //fit scene into view
    ui->graphicsView->fitInView(0,0,350*logic->xmax,350*logic->ymax,Qt::KeepAspectRatio);

    // render various items into view
    showProtagonist();
    showHealthpacks();
    showEnemies();

    // set the starting position of the protagonist/algorithm
    logic->setStart(0,0);

    //set the destination for the algorithm and make the Tile red
    logic->setDestination(0,0);

    destView = screen->scene->addRect(256*logic->xDest, 256*logic->yDest, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255)));

    updateStats();
    //indicateDestination(logic->xDest, logic->yDest);
    //item->setFlag(QGraphicsItem::ItemIsSelectable, true);

}

void MainWindow::ItemSelected()
{

    // Change destination to selected tileView
    qDebug() << "Selection changed";
    auto selected = screen->scene->selectedItems();
    auto x = 0;
    auto y = 0;
    if (!selected.empty()){
        x = selected[0]->x()/256;
        y = selected[0]->y()/256;
    }

    logic->xDest = x;
    logic->yDest = y;
    indicateDestination(x,y);

}
