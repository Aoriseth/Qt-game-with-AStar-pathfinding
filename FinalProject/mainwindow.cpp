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

    int weight = ui->lineEdit->text().toInt();
    if(!mapLoaded){return;} // Don't play if there is no map loaded
//    logic->strat();

//    return;
    while(!logic->isAllDefeated()){ //check if all enemies are defeated
        auto finished = false;
        auto it = logic->getClosestEnemy();
        Enemy closestEnemy = **it;
        while((*it)->getValue()>logic->getHealth()){
            qDebug()<<"Health is not enough to defeat an enemy, go for healthpack";
            if(logic->healthpacks.size()==0){
                qDebug()<<"Quit: NO healthpack left!";
                return;
            }
            Tile healthpack = logic->getClosestHealthpack();
            logic->setDestination(healthpack.getXPos(),healthpack.getYPos());
            logic->setWeight(weight);
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
                    qDebug()<<"Succeed to get a healthpack!";
                    qDebug()<<"New Health is "<<logic->getHealth();
                    logic->setStart(logic->xDest,logic->yDest);
                    it = logic->getClosestEnemy();
                    closestEnemy = **it;
                }

            }
        }
//        (*it)->setDefeated(true); //mark this enemy as defeated
        logic->killEnemy(*it);

        logic->setDestination(closestEnemy.getXPos(),closestEnemy.getYPos());
        logic->setWeight(weight);
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
            qDebug()<<"Succeed to kill an enemy!";
            logic->setStart(logic->xDest,logic->yDest);
        }else{  //Path not found
            logic->setStart(logic->getProtagonistX(),logic->getProtagonistY());
        }
    }
    logic->setDestination(0,0);
    indicateDestination(0,0);
    qDebug()<<"Game Win! All the enemies are defeated!";
    screen->clearPath();

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

void MainWindow::refreshWindow()
{
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::updateStats(float energy, float health){
    ui->energyBar->setValue(energy);
    ui->healthBar->setValue(health);
//    qDebug()<<"Energy is "<<logic->getEnergy();

}


void MainWindow::mapLoad()
{
    mapLoaded = true;
    //clear logic lists and refresh the scene
    logic->clearLists();
    refreshScene();
    connect(logic, SIGNAL(changeStats(float, float)), this, SLOT(updateStats(float,float))); // connect signals to update protagonist stats
    connect(screen, SIGNAL(updatePath()), this, SLOT(refreshWindow())); // connect signals to update path visuals

    //loads world into scene
    logic->loadWorld(path,screen->sceneView);

    //fit scene into view
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

    screen->destView = screen->sceneView->addRect(256*logic->xDest, 256*logic->yDest, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 255,255)));
    screen->destView->setScale(0.00390625);
    screen->destView->setOpacity(0.5);
}

void MainWindow::OpenMap()
{
    path = QFileDialog::getOpenFileName(this,tr("Select map"));
    mapLoad();

}

void MainWindow::ItemSelected(int x, int y)
{
    screen->clearPath();
    logic->xDest = x;
    logic->yDest = y;
    indicateDestination(x,y);
}

void MainWindow::updatePosition(int x, int y)
{
    screen->setProtagonistPosition(x,y);
    ui->graphicsView->viewport()->repaint();
    auto speed = ui->horizontalSlider->value();
    std::this_thread::sleep_for(std::chrono::milliseconds(10+speed));
}

void MainWindow::ReloadMap()
{
    mapLoad();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   if(!mapLoaded){return;}
   ui->graphicsView->fitInView(logic->worldView,Qt::KeepAspectRatio);
}
