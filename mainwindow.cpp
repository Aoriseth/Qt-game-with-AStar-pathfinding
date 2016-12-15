#include "mainwindow.h"
#include "ui_mainwindow.h"


QGraphicsScene * scene;
QString path = ":/resources/maze_5_5.png";
QGraphicsItem * destView;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    game logic();
    ui->setupUi(this);

}

void MainWindow::addItemToScene(QGraphicsPixmapItem* item, int x, int y){
    //auto item = std::make_shared<QGraphicsPixmapItem>(new QGraphicsPixmapItem(QPixmap::fromImage(image)));
    item->setScale(1);
    item->setPos(256*x,256*y);
    scene->addItem(item);
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
        logic.setWeight(10);
        finished = logic.calcPath_Dijkstra();
        break;
    case 1:
        finished = logic.calcPath_BreadthFirst();
        break;
    case 2:
        finished = logic.calcPath_BestFirst();
        break;
    default:
        break;
    }

    // Move the protagonist based on the calculated path
    if(finished){
        while(logic.route.size()){
            auto tile = logic.route.pop();
            protagonistView->setPos(256*(tile->getXPos()),256*(tile->getYPos()));
            ui->graphicsView->viewport()->repaint();
            logic.protagonist->setEnergy(logic.protagonist->getEnergy()-1);
            updateStats();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        logic.setStart(logic.xDest,logic.yDest);
    }else{
        logic.setStart(logic.protagonist->getXPos(),logic.protagonist->getYPos());
    }
}

void MainWindow::execute_strategy()
{
    auto finished = false;

    // Check the chosen algorithm
    switch (ui->comboBox->currentIndex()) {
    case 0:
        logic.setWeight(10);
        finished = logic.calcPath_Dijkstra();
        break;
    case 1:
        finished = logic.calcPath_BreadthFirst();
        break;
    case 2:
        finished = logic.calcPath_BestFirst();
        break;
    default:
        break;
    }

    // Move the protagonist based on the calculated path
    if(finished){
        while(logic.route.size()){
            auto tile = logic.route.pop();
            protagonistView->setPos(256*(tile->getXPos()),256*(tile->getYPos()));
            ui->graphicsView->viewport()->repaint();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        logic.setStart(logic.xDest,logic.yDest);
    }else{
        logic.setStart(logic.protagonist->getXPos(),logic.protagonist->getYPos());
    }



}

void MainWindow::refreshScene(){
    scene = new QGraphicsScene(this);
    connect(scene, SIGNAL(selectionChanged()), this, SLOT(ItemSelected()));
    ui->graphicsView->setScene(scene);

    //scene->setSceneRect(scene->itemsBoundingRect());
}

void MainWindow::showProtagonist(){
    QImage image(":/resources/Pixel-mario.png");
    protagonistView = new QGraphicsPixmapItem(QPixmap::fromImage(image));
    protagonistView->setZValue(3);
    protagonistView->setScale(1);
    protagonistView->setPos(logic.protagonist->getXPos(),logic.protagonist->getYPos());
    scene->addItem(protagonistView);
}

void MainWindow::showHealthpacks(){
    for(auto& healthpack: logic.healthpacks){
        int x = healthpack->getXPos();
        int y = healthpack->getYPos();
        QImage image(":/resources/Supermushroom.png");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        addItemToScene(item, x, y);
        healthpackItems.push_back(item);
    }
}

void MainWindow::showEnemies(){
    for(auto& enemy: logic.enemies){
        int x = enemy->getXPos();
        int y = enemy->getYPos();
        QImage image(":/resources/goomba.gif");
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
        addItemToScene(item, x, y);
        enemyItems.push_back(item);
    }
}

void MainWindow::indicateDestination(int x, int y){
    destView->setPos(256*x,256*y);
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::updateStats(){
    ui->energyBar->setValue(logic.protagonist->getEnergy());
    ui->healthBar->setValue(logic.protagonist->getHealth());
    qDebug()<<"Health is "<<logic.protagonist->getEnergy();

}


void MainWindow::OpenMap()
{
    mapLoaded = true;
    //clear logic lists and refresh the scene
    logic.clearLists();
    refreshScene();

    //set path chosen by user
    QString path = QFileDialog::getOpenFileName(this,tr("Select map"));

    //loads world into scene
    logic.loadWorld(path,scene);

    //fit scene into view
    ui->graphicsView->fitInView(0,0,350*logic.xmax,350*logic.ymax,Qt::KeepAspectRatio);

    // render various items into view
    showProtagonist();
    showHealthpacks();
    showEnemies();

    // set the starting position of the protagonist/algorithm
    logic.setStart(0,0);

    //set the destination for the algorithm and make the Tile red
    logic.setDestination(0,0);

    destView = scene->addRect(256*logic.xDest, 256*logic.yDest, 256, 256, QPen(QColor(0, 0, 0,0)), QBrush(QColor(255, 0, 0,255)));

    updateStats();
    //indicateDestination(logic.xDest, logic.yDest);
    //item->setFlag(QGraphicsItem::ItemIsSelectable, true);

}

void MainWindow::ItemSelected()
{

    // Change destination to selected tileView
    qDebug() << "Selection changed";
    auto selected = scene->selectedItems();
    auto x = 0;
    auto y = 0;
    if (!selected.empty()){
        x = selected[0]->x()/256;
        y = selected[0]->y()/256;
    }

    logic.xDest = x;
    logic.yDest = y;
    indicateDestination(x,y);

}
