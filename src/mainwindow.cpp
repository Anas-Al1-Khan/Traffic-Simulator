#include "mainwindow.h"
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QStatusBar>
#include <QPainter>
#include <QTimer>
#include <cstdlib> // for rand()

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Traffic Management Simulator - Day 3: Queue Integration");
    resize(900, 700);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 900, 700);
    m_scene->setBackgroundBrush(QColor(25, 28, 35));

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    setCentralWidget(m_view);

    setupData();
    buildScene();
    
    // Timer to simulate cars arriving every 1.5 seconds
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::simulateArrival);
    timer->start(1500);

    statusBar()->showMessage("Day 3 - Live queue sizes updating via QTimer");
}

void MainWindow::setupData()
{
    // Populate Roads dynamically
    m_roads.push_back({1, 150, 150, 750, 150}); // Top
    m_roads.push_back({2, 150, 150, 150, 550}); // Left
    m_roads.push_back({3, 750, 150, 750, 550}); // Right
    m_roads.push_back({4, 150, 550, 750, 550}); // Bottom
    m_roads.push_back({5, 150, 150, 750, 550}); // Diagonal

    // Populate Intersections dynamically
    m_intersections.push_back(Intersection(1, 150, 150, "A"));
    m_intersections.push_back(Intersection(2, 750, 150, "B"));
    m_intersections.push_back(Intersection(3, 150, 550, "C"));
    m_intersections.push_back(Intersection(4, 750, 550, "D"));
    m_intersections.push_back(Intersection(5, 450, 350, "E"));
}

void MainWindow::buildScene()
{
    // 1. Draw all roads
    for (size_t i = 0; i < m_roads.size(); ++i) {
        drawRoad(m_roads[i]);
    }

    // 2. Draw all intersections & their queue labels
    for (size_t i = 0; i < m_intersections.size(); ++i) {
        drawIntersection(m_intersections[i]);
    }
}

void MainWindow::drawIntersection(Intersection& intersection)
{
    const qreal radius = 32.0;

    // Draw Circle
    QGraphicsEllipseItem* circle = m_scene->addEllipse(
        intersection.x - radius, intersection.y - radius,
        2 * radius, 2 * radius,
        QPen(QColor(0, 200, 255), 3), 
        QBrush(QColor(35, 75, 115))
    );
    circle->setZValue(10);

    // Draw Letter Label
    QGraphicsSimpleTextItem* text = m_scene->addSimpleText(intersection.label);
    QFont f = text->font();
    f.setBold(true);
    f.setPointSize(14);
    text->setFont(f);
    text->setBrush(Qt::white);
    QRectF br = text->boundingRect();
    text->setPos(intersection.x - br.width() / 2.0, intersection.y - br.height() / 2.0);
    text->setZValue(11);

    // Draw "Waiting: 0" Label (Below the intersection)
    QGraphicsTextItem* queueLabel = m_scene->addText("Waiting: 0");
    queueLabel->setDefaultTextColor(Qt::yellow);
    QFont qf = queueLabel->font();
    qf.setPointSize(10);
    queueLabel->setFont(qf);
    queueLabel->setPos(intersection.x - 30, intersection.y + radius + 5);
    queueLabel->setZValue(11);

    // Store the pointer so we can update it later
    m_queueLabels.push_back(queueLabel);
}

void MainWindow::drawRoad(Road& road)
{
    QPen pen(QColor(200, 200, 200), 20);
    pen.setCapStyle(Qt::RoundCap);
    QGraphicsLineItem* line = m_scene->addLine(road.x1, road.y1, road.x2, road.y2, pen);
    line->setZValue(0);
}

void MainWindow::simulateArrival()
{
    // Pick a random intersection (0 to 4)
    int randomIndex = rand() % m_intersections.size();
    Intersection& target = m_intersections[randomIndex];

    // Add a car (represented by a random ID) to the queue
    int carId = rand() % 1000 + 1;
    
    // Call YOUR custom queue's enqueue method
    if (target.waitingCars.enqueue(carId)) {
        updateQueueLabels();
        statusBar()->showMessage(QString("Car %1 arrived at Intersection %2").arg(carId).arg(target.label));
    } else {
        statusBar()->showMessage(QString("Intersection %1 is FULL!").arg(target.label));
    }
}

void MainWindow::updateQueueLabels()
{
    for (size_t i = 0; i < m_intersections.size(); ++i) {
        int count = m_intersections[i].waitingCars.size();
        m_queueLabels[i]->setPlainText(QString("Waiting: %1").arg(count));
    }
}