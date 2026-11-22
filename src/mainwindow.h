#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QTimer>
#include <vector>
#include "Graph.h"
#include "Simulation.h"
#include "vehicleitem.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;

private slots:
    void updateSimulation();
    void onCloseRoadClicked();

private:
    void drawMap();
    QPointF getNodePosition(int nodeId);

    QLabel* statsLabel;
    QGraphicsScene* scene;
    QGraphicsView* view;
    
    Graph* map;
    Simulation* sim;
    
    int tickCounter;
    QTimer* timer;
    bool isCDRoadClosed;

    // For Day 4/5 Animation
    std::vector<VehicleItem*> m_activeVehicles;
    std::vector<int> m_lastLocations; // Tracks where cars were last tick
};

#endif // MAINWINDOW_H