#pragma once
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QLabel>
#include <QPushButton>
#include "Graph.h"
#include "Simulation.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void updateSimulation(); // Runs every tick
    void onCloseRoadClicked();

private:
    int tickCounter;
    bool isCDRoadClosed; // Flag to track road state
    QGraphicsScene* scene;
    QGraphicsView* view;
    QTimer* timer;
    QLabel* statsLabel; // Dashboard text

    Graph* map;
    Simulation* sim;

    QPointF getNodePosition(int nodeId);
    void drawMap();
    void drawVehicles(); // NEW: Draws the cars
};