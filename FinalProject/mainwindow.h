#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "world.h"
#include "world_global.h"
#include <vector>
#include <iostream>
#include <QString>
#include <thread>
#include <QFileDialog>
#include "game.h"
#include "view.h"
#include <chrono>
#include <QElapsedTimer>


class game;
class view;

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

    void setLogic(std::shared_ptr<game> pass);
    void setView(std::shared_ptr<view> pass);


    void indicateDestination(int x, int y);

protected:
    void resizeEvent(QResizeEvent *event);

private:
    QString path;
    std::shared_ptr<game> logic;
    std::shared_ptr<view> screen;
    void refreshScene();

    bool mapLoaded=false;
    Tile killEnemy();

    void mapLoad();

private slots:
    void updateStats(float energy, float health);
    void gotoDestination();
    void executeStrategy();
    void OpenMap();
    void refreshWindow();
    void ItemSelected(int x, int y);
    void updatePosition(int x, int y);
    void ReloadMap();
    void setWeight(int x);
};

#endif // MAINWINDOW_H
