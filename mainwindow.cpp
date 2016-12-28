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
    QElapsedTimer timer;
    timer.start();
    int weight = ui->lineEdit->text().toInt();
    logic->setWeight(weight);
    if(!mapLoaded){return;} // Don't play if there is no map loaded
    while(!logic->isAllDefeated()){ //check if all enemies are defeated
        while(!(logic->isDefeatable())){//check if there is a defeatable enemy before calculate the path for closest enemy
            qDebug()<<"Health is not enough to defeat an enemy, go for healthpack";
            if(logic->healthpacks.size()==0){
                qDebug()<<"Quit: NO healthpack left!";
                return;
            }
            if(!logic->goForHealthpack()){
                return; //quit the loop
            }
        }
        if(!logic->goForEnemy()){
            return; //quit the loop
        }

    }
    logic->setDestination(0,0);
    indicateDestination(0,0);
    qDebug()<<"Game Win! All the enemies are defeated!";
    screen->clearPath();
    qDebug() << "The run_strategy operation took" << timer.elapsed() << "milliseconds";
}

void MainWindow::refreshScene(){
    screen->sceneView = new Scene(this);
    ui->graphicsView->setScene(screen->sceneView);
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
}


void MainWindow::mapLoad()
{
    mapLoaded = true;
    // Create a new scene
    refreshScene();
    connect(screen->sceneView, SIGNAL(locationClicked(int,int)), this, SLOT(ItemSelected(int,int)));
    connect(logic, SIGNAL(changeStats(float, float)), this, SLOT(updateStats(float,float))); // connect signals to update protagonist stats
    connect(screen, SIGNAL(updateViewport()), this, SLOT(refreshWindow())); // connect signals to update path visuals

    //loads world into scene and cleanup data from the previous world
    logic->clearLists();
    screen->clearLists();
    logic->loadWorld(path,screen->sceneView);

    //fit scene into view
    ui->graphicsView->fitInView(logic->worldView,Qt::KeepAspectRatio);

    // render various items into view
    screen->showProtagonist();
    screen->showHealthpacks();
    screen->showEnemies();

    connect(logic->getProtagonist(), SIGNAL(posChanged(int,int)), this, SLOT(updatePosition(int, int)));

    // set the starting position of the protagonist/algorithm
    logic->setStart(0,0);

    screen->destView = screen->sceneView->addRect(256*logic->xDest, 256*logic->yDest, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 255,255)));
    screen->destView->setScale(0.00390625);
    screen->destView->setOpacity(0.5);
}

void MainWindow::OpenMap()
{
    path = QFileDialog::getOpenFileName(this,tr("Select map"));
    if(!path.isEmpty()){
        mapLoad();
    }


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
