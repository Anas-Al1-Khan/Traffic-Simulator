#include "mainwindow.h"
#include <QPen>
#include <QBrush>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // 1. Setup the Main Layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);

    // 2. Create Dashboard Label
    statsLabel = new QLabel("Dashboard loading...", this);
    statsLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold; padding: 10px;");
    layout->addWidget(statsLabel);

    // 3. Setup the Visual Scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    scene->setBackgroundBrush(QColor(30, 30, 30)); // Dark theme

    view = new QGraphicsView(scene, this);
    layout->addWidget(view);

    // Add Close Road Button
    QPushButton* closeRoadBtn = new QPushButton("Close Road C-D", this);
    closeRoadBtn->setStyleSheet("background-color: #cc0000; color: white; font-weight: bold; padding: 10px;");
    layout->addWidget(closeRoadBtn);
    connect(closeRoadBtn, &QPushButton::clicked, this, &MainWindow::onCloseRoadClicked);

    isCDRoadClosed = false; // Initialize flag
    setCentralWidget(centralWidget);
    setWindowTitle("Traffic Management Simulator - Final Prototype");

    // 4. Setup Logic (Using the 5 nodes A, B, C, D, E)
    map = new Graph(5);
    map->addRoad(0, 1, 4); // A - B
    map->addRoad(0, 2, 2); // A - C
    map->addRoad(1, 3, 5); // B - D
    map->addRoad(2, 3, 1); // C - D
    map->addRoad(0, 4, 3); // A - E
    map->addRoad(4, 3, 3); // E - D

    sim = new Simulation(map, 20);

    // Spawn some normal cars
    sim->spawnVehicle(0, 3, false);
    sim->spawnVehicle(0, 3, false);
    // Spawn an emergency vehicle
    sim->spawnVehicle(0, 3, true);

    // 5. Draw initial state
    drawMap();

    // Initialize tracking variables for animation
    for (int i = 0; i < sim->getVehicleCount(); i++) {
        m_lastLocations.push_back(sim->getVehicleLocation(i));
    }

    // 6. Start Timer (1.5 seconds)
    tickCounter = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    timer->start(1500); 
}

MainWindow::~MainWindow() {
    delete map;
    delete sim;
}

void MainWindow::updateSimulation() {
    sim->advanceTime();
    tickCounter++;

    // Spawn a new vehicle every 2 ticks
    if (tickCounter % 2 == 0) {
        sim->spawnVehicle(0, 3, false); 
        m_lastLocations.push_back(-2); // -2 means "newly spawned, don't animate yet"
        
        if (tickCounter % 4 == 0) {
            sim->spawnVehicle(0, 3, true); 
            m_lastLocations.push_back(-2);
        }
    }

    // Update the dashboard text
    statsLabel->setText(QString("Total Vehicles: %1 | Completed Trips: %2 | (Emergency = Flashing Blue)").arg(sim->getVehicleCount()).arg(sim->getCompletedTrips()));

    // --- ANIMATION BRIDGE ---
    // Check if any cars moved to a new node, and spawn an animation for them
    for (int i = 0; i < sim->getVehicleCount(); i++) {
        int currentNode = sim->getVehicleLocation(i);
        int lastNode = m_lastLocations[i];

        if (currentNode != -1 && lastNode != -1 && currentNode != lastNode) {
            // Car moved! Animate it sliding from lastNode to currentNode
            QPointF startPos = getNodePosition(lastNode);
            QPointF endPos = getNodePosition(currentNode);
            bool isEmergency = sim->isEmergencyVehicle(i);

            VehicleItem* vehicle = new VehicleItem(i, startPos.x(), startPos.y(), endPos.x(), endPos.y(), isEmergency);
            scene->addItem(vehicle);
            m_activeVehicles.push_back(vehicle);
            vehicle->startMoving();
        }
        
        // Update our tracker
        m_lastLocations[i] = currentNode;
    }
}

QPointF MainWindow::getNodePosition(int nodeId) {
    switch (nodeId) {
    case 0: return QPointF(150, 150); // A
    case 1: return QPointF(650, 150); // B
    case 2: return QPointF(150, 450); // C
    case 3: return QPointF(650, 450); // D
    case 4: return QPointF(400, 300); // E
    default: return QPointF(0, 0);
    }
}

void MainWindow::drawMap() {
    // Draw roads
    QPen roadPen(Qt::gray, 8); 

    scene->addLine(150, 150, 650, 150, roadPen); // A-B
    scene->addLine(150, 150, 150, 450, roadPen); // A-C
    scene->addLine(650, 150, 650, 450, roadPen); // B-D

    // C-D Road (Dynamic)
    if (isCDRoadClosed) {
        QPen closedPen(Qt::red, 8);
        scene->addLine(150, 450, 650, 450, closedPen); // Draw red
    }
    else {
        scene->addLine(150, 450, 650, 450, roadPen); // Draw gray
    }

    scene->addLine(150, 150, 400, 300, roadPen); // A-E
    scene->addLine(400, 300, 650, 450, roadPen); // E-D

    // Draw Intersections
    QPen nodePen(Qt::cyan, 3);
    QBrush nodeBrush(Qt::darkBlue);
    QFont font = scene->font();
    font.setPointSize(16);
    font.setBold(true);

    for (int i = 0; i < 5; i++) {
        QPointF pos = getNodePosition(i);
        scene->addEllipse(pos.x() - 25, pos.y() - 25, 50, 50, nodePen, nodeBrush);
        QGraphicsTextItem* text = scene->addText(QString(QChar('A' + i)), font);
        text->setDefaultTextColor(Qt::white);
        text->setPos(pos.x() - 10, pos.y() - 15);
    }
}

void MainWindow::onCloseRoadClicked() {
    // Clear the scene and redraw the map to show the red road
    scene->clear();
    
    if (!isCDRoadClosed) {
        map->closeRoad(2, 3); // Close C-D
        isCDRoadClosed = true;
    }
    else {
        map->addRoad(2, 3, 1); // Reopen C-D
        isCDRoadClosed = false;
    }
    drawMap();
}