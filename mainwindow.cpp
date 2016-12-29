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
}

void MainWindow::gotoDestination()
{
    if(!mapLoaded){return;} // Don't play if there is no map loaded

    int weight = ui->lineEdit->text().toInt(); // read weight from textbox
    logic->setWeight(weight);

    // calculate path and move protagonist
    logic->go();
}

void MainWindow::executeStrategy()
{

    int weight = ui->lineEdit->text().toInt();
    logic->setWeight(weight);
    if(!mapLoaded){return;} // Don't play if there is no map loaded

    logic->strat();
    screen->clearPath();

}

void MainWindow::refreshScene(){
    screen->sceneView.reset();
    screen->sceneView = std::make_shared<Scene>(this);
    ui->graphicsView->setScene(screen->sceneView.get());
}


void MainWindow::setLogic(std::shared_ptr<game> test)
{
    logic = test;
}

void MainWindow::setView(std::shared_ptr<view> pass)
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


void MainWindow::connectAll()
{
    connect(logic.get(), SIGNAL(destinationChanged(int,int)), screen.get(), SLOT(indicateDestination(int,int)));
    connect(screen->sceneView.get(), SIGNAL(locationClicked(int,int)), this, SLOT(ItemSelected(int,int)));
    connect(screen->sceneView.get(), SIGNAL(keyClicked(int)), logic.get(), SLOT(movePro(int)));
    connect(logic.get(), SIGNAL(changeStats(float, float)), this, SLOT(updateStats(float,float))); // connect signals to update protagonist stats
    connect(screen.get(), SIGNAL(updateViewport()), this, SLOT(refreshWindow()));
}

void MainWindow::mapLoad()
{
    mapLoaded = true;

    // Create a new scene and connect signals
    refreshScene();
    connectAll();

    //loads world into scene and cleanup data from the previous world
    screen->clearLists();
    logic->loadWorld(path);

    //fit scene into view
    ui->graphicsView->fitInView(screen->getWorldView().get(),Qt::KeepAspectRatio);

    // render various items into view
    screen->showProtagonist();
    screen->showHealthpacks();
    screen->showEnemies();

    connect(logic->getProtagonist().get(), SIGNAL(posChanged(int,int)), this, SLOT(updatePosition(int, int)));

    // set the starting position of the protagonist/algorithm
    logic->setStart(0,0);

    screen->destView = std::shared_ptr<QGraphicsItem>(screen->sceneView->addRect(256*logic->xDest, 256*logic->yDest, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(0, 0, 255,255))));
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
    logic->setDestination(x,y);
}



void MainWindow::updatePosition(int x, int y)
{
    screen->setProtagonistPosition(x,y);
    ui->graphicsView->viewport()->repaint();
    auto speed = ui->horizontalSlider->value();
    std::this_thread::sleep_for(std::chrono::milliseconds(99-speed));
}

void MainWindow::ReloadMap()
{
    mapLoad();
}

void MainWindow::setWeight(int x)
{
    switch (x) {
    case 0:
        ui->lineEdit->setText(QString::number(1));
        break;
    case 1:
        ui->lineEdit->setText(QString::number(100));
        break;
    case 2:
        ui->lineEdit->setText(QString::number(0));
        break;
    default:
        // default to astar, should never happen
        ui->lineEdit->setText(QString::number(0.1));
        break;
    }
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   if(!mapLoaded){return;}
   ui->graphicsView->fitInView(screen->getWorldView().get(),Qt::KeepAspectRatio);
}
