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

void MainWindow::addItemToScene(QImage image, int x, int y){
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(QPixmap::fromImage(image));
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
    if(logic.calcPath_BestFirst()){
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
        QImage image1(":/resources/Supermushroom.png");
        addItemToScene(image1, x, y);
    }
}

void MainWindow::showEnemies(){
    for(auto& enemy: logic.enemies){
        int x = enemy->getXPos();
        int y = enemy->getYPos();
        QImage image2(":/resources/goomba.gif");
        addItemToScene(image2, x, y);
    }
}

void MainWindow::indicateDestination(int x, int y){
    destView->setPos(256*x,256*y);
    ui->graphicsView->viewport()->repaint();
}

void MainWindow::OpenMap()
{
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


    //indicateDestination(logic.xDest, logic.yDest);
    //item->setFlag(QGraphicsItem::ItemIsSelectable, true);

}

void MainWindow::readDestination()
{

    auto x = ui->lineEditX->text();
    logic.xDest = x.split(" ")[0].toInt();
    auto y = ui->lineEditY->text();
    logic.yDest = y.split(" ")[0].toInt();
    qDebug() << "xdest = " << logic.xDest << " and ydest = " << logic.yDest;
    qDebug() << logic.currentNodes.size();
    indicateDestination(logic.xDest, logic.yDest);

}
