#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsTextItem>
#include <vector>
#include "models.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private slots:
    void simulateArrival(); // Triggered by QTimer

private:
    void setupData();
    void buildScene();
    void updateQueueLabels();
    
    void drawIntersection(Intersection& intersection);
    void drawRoad(Road& road);

    QGraphicsScene* m_scene = nullptr;
    QGraphicsView*  m_view  = nullptr;

    // Core Data using std::vector
    std::vector<Road> m_roads;
    std::vector<Intersection> m_intersections;
    
    // GUI Pointers for fast updates
    std::vector<QGraphicsTextItem*> m_queueLabels;
};

#endif // MAINWINDOW_H