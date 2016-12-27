#include "mainwindow.h"
#include "ui_mainwindow.h"





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

void MainWindow::gotoDestination()
{
    if(!mapLoaded){return;} // Don't play if there is no map loaded
    auto finished = false; // Set variable for checking if an algorithm successfully finished
    int weight = ui->lineEdit->text().toInt(); // read weight from textbox
//    qDebug() << "weight is " << weight;

    // Check the chosen algorithm
    switch (ui->comboBox->currentIndex()) {
    case 0:
        logic->setWeight(weight);
        finished = logic->calcPath_AStar();
        break;
    case 1:
        finished = logic->calcPath_BreadthFirst();
        break;
    case 2:
        finished = logic->calcPath_BestFirst();
        break;
    default:
        logic->setWeight(weight);
        finished = logic->calcPath_AStar(); // default to AStar, should never happen
        break;
    }

    // Move the protagonist based on the calculated path
    if(finished){
        logic->MoveProtagonist();
        logic->setStart(logic->xDest,logic->yDest);
    }else{
        logic->setStart(logic->getProtagonistX(),logic->getProtagonistY());
    }
}

void MainWindow::executeStrategy()
{
    if(!mapLoaded){return;} // Don't play if there is no map loaded

    while(!logic->isAllDefeated()){ //check if all enemies are defeated
        auto finished = false;
        auto it = logic->getClosestEnemy();
        Enemy closestEnemy = **it;
        if((*it)->getValue()>logic->getHealth()){
            qDebug()<<"Health is not enough to defeat an enemy, go for healthpack";
            if(logic->healthpacks.size()==0){
                qDebug()<<"Quit: NO healthpack left!";
                return;
            }
            Tile healthpack = logic->getClosestHealthpack();
            logic->setDestination(healthpack.getXPos(),healthpack.getYPos());
            logic->setWeight(5);
            bool find = logic->calcPath_AStar();
            if(find){
                float requiredEnergy = logic->getMoveCost();
                if(requiredEnergy>logic->getEnergy()){
                    qDebug()<<"Game failed! Not enough energy to closest healthpack!Energy required: "<<requiredEnergy;
                    return; //quit the loop
                }else{
                    float newHealth = logic->getHealth()+10.0*healthpack.getValue();
                    if(newHealth > 100) newHealth = 100;
                    logic->setHealth(newHealth);
                    logic->setMoveCost(0.0f);
                    // Move the protagonist based on the calculated path
                    logic->MoveProtagonist();
//                    while(logic->route.size()){qDebug()<<"Go to Healthpack!";
//                        auto tile = logic->route.pop();
//                        screen->setProtagonistPosition((tile->getXPos()),(tile->getYPos()));
//                        float newEnergy = logic->getEnergy()-1 - logic->getWeight()*(1-tile->getValue());
//                        logic->setEnergy(newEnergy);
//                    }
                    qDebug()<<"Succeed to get a healthpack!";
                    qDebug()<<"New Health is "<<logic->getHealth();
                    logic->setStart(logic->xDest,logic->yDest);
                    auto it = logic->getClosestEnemy();
                    closestEnemy = **it;
                    if((*it)->getValue()>logic->getHealth()){
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
        finished = logic->calcPath_AStar();
        if(finished){  //Path found
            float requiredEnergy = logic->getMoveCost();
            if(requiredEnergy>logic->getEnergy()){
                qDebug()<<"Game failed! Not enough energy to next enemy!Energy required: "<<requiredEnergy;
                return; //quit the loop
            }else{
                float newHealth = logic->getHealth()-closestEnemy.getValue();
                logic->setHealth(newHealth);
                qDebug()<<"Health is "<<logic->getHealth();
                logic->setEnergy(100);
                logic->setMoveCost(0.0f);
            }
            // Move the protagonist based on the calculated path
            logic->MoveProtagonist();
//            while(logic->route.size()){
//                auto tile = logic->route.pop();
//                screen->setProtagonistPosition((tile->getXPos()),(tile->getYPos()));
//                ui->graphicsView->viewport()->repaint();
//                float newEnergy = logic->getEnergy()-1 - logic->getWeight()*(1-tile->getValue());
//                logic->setEnergy(newEnergy);
//            }
            qDebug()<<"Succeed to kill an enemy!";
            logic->setStart(logic->xDest,logic->yDest);
        }else{  //Path not found
            logic->setStart(logic->getProtagonistX(),logic->getProtagonistY());
        }
    }
    logic->setDestination(0,0);
    indicateDestination(0,0);
    qDebug()<<"Game Win! All the enemies are defeated!";

}

void MainWindow::refreshScene(){
    screen->sceneView = new Scene(this);
    connect(screen->sceneView, SIGNAL(locationClicked(int,int)), this, SLOT(ItemSelected(int,int)));
    ui->graphicsView->setScene(screen->sceneView);

    //scene->setSceneRect(scene->itemsBoundingRect());
}

void MainWindow::indicateDestination(int x, int y){
    screen->destView->setPos(x,y);
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

void MainWindow::updateStats(float energy, float health){
    ui->energyBar->setValue(energy);
    ui->healthBar->setValue(health);
//    qDebug()<<"Energy is "<<logic->getEnergy();

}


void MainWindow::OpenMap()
{
    mapLoaded = true;
    //clear logic lists and refresh the scene
    logic->clearLists();
    refreshScene();
    connect(logic, SIGNAL(changeStats(float, float)), this, SLOT(updateStats(float,float)));
    //set path chosen by user
    QString path = QFileDialog::getOpenFileName(this,tr("Select map"));

    //loads world into scene
    logic->loadWorld(path,screen->sceneView);

    //fit scene into view
    ui->graphicsView->fitInView(0,0,350*logic->xmax,350*logic->ymax,Qt::KeepAspectRatio);
    ui->graphicsView->fitInView(logic->worldView,Qt::KeepAspectRatio);

    // render various items into view
    screen->showProtagonist();
    connect(logic->getProtagonist(), SIGNAL(posChanged(int,int)), this, SLOT(updatePosition(int, int)));
    screen->showHealthpacks();
    screen->showEnemies();

    // set the starting position of the protagonist/algorithm
    logic->setStart(0,0);

    //set the destination for the algorithm and make the Tile red
    logic->setDestination(0,0);

    screen->destView = screen->sceneView->addRect(256*logic->xDest, 256*logic->yDest, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255)));
    screen->destView->setScale(0.00390625);
    //indicateDestination(logic->xDest, logic->yDest);
    //item->setFlag(QGraphicsItem::ItemIsSelectable, true);

}

void MainWindow::ItemSelected(int x, int y)
{

    // Change destination to selected tileView
//    qDebug() << "Selection changed";
//    auto selected = screen->sceneView->selectedItems();
//    auto x = 0;
//    auto y = 0;
//    if (!selected.empty()){
//        x = selected[0]->x()/256;
//        y = selected[0]->y()/256;
//    }

    logic->xDest = x;
    logic->yDest = y;
    indicateDestination(x,y);

}

void MainWindow::updatePosition(int x, int y)
{
    screen->setProtagonistPosition(x,y);
    ui->graphicsView->viewport()->repaint();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   if(!mapLoaded){return;}
   ui->graphicsView->fitInView(logic->worldView,Qt::KeepAspectRatio);
}
