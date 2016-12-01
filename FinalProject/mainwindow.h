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
#include <QQueue>
#include <QDebug>
#include <QSet>
#include <algorithm>
#include <QStack>
#include <chrono>
#include <thread>
#include <QFileDialog>

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
    void drawScene();
    void addItemToScene(QImage image, int x, int y);
private:
    int xmax;
    int ymax;

private slots:
    void play_clicked();
    void OpenMap();
};

#endif // MAINWINDOW_H
