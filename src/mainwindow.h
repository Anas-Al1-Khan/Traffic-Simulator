#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem> // Added for the road toggling fix
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

    // Pointers to the C-D road so we can turn it red without clearing the screen
    QGraphicsLineItem* m_cdRoadLine = nullptr;
    QGraphicsLineItem* m_cdLaneLine = nullptr;

    // For Animation
    std::vector<VehicleItem*> m_activeVehicles;
    std::vector<int> m_lastLocations; 
};

#endif // MAINWINDOW_H