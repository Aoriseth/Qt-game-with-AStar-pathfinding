#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "world.h"
#include "world_global.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QFrame>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <iostream>
#include <math.h>
#include <QGraphicsPixmapItem>
#include "node.h"
#include <vector>
#include <iostream>
#include <QString>
#include <QListIterator>
#include <QDebug>
#include <algorithm>
#include <chrono>
#include <thread>
#include <QFileDialog>
#include "game.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow *ui;
    bool findMyPath(int x, int y);
    void pathTrace();
    void addItemToScene(QGraphicsPixmapItem* item, int x, int y);
    void indicateDestination(int x, int y);
    std::vector<QGraphicsPixmapItem*> enemyItems;
    std::vector<QGraphicsPixmapItem*> healthpackItems;

    void updateStats();
private:
    game logic;
    void refreshScene();
    QGraphicsPixmapItem* protagonistView;
    void showProtagonist();
    void showHealthpacks();
    void showEnemies();
    bool mapLoaded=false;
    //static bool dist_compare(std::unique_ptr<Enemy> &a, std::unique_ptr<Enemy> &b);
    Tile killEnemy();
//    struct doCompare : std::binary_function<Tile, Tile, bool>
//       {
//           doCompare( const MainWindow& win ) : _MainWindow(win) { }
//           const MainWindow& _MainWindow;

//           bool operator()( const Tile & a, const Tile & b  )
//           {
//               return _MainWindow.dist_compare(a, b);
//           }
//       };

private slots:
    void play_clicked();
    void execute_strategy();
    void OpenMap();
    void ItemSelected();
};

#endif // MAINWINDOW_H
