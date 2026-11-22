#include "mainwindow.h"
#include <QPen>
#include <QBrush>
#include <QVBoxLayout>
#include <QWidget>
#include <QGraphicsEllipseItem>

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

    setCentralWidget(centralWidget);
    setWindowTitle("Traffic Management Simulator - Prototype");

    // 4. Setup Logic (Using the 5 nodes A, B, C, D, E from your teammate's map)
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

    // 6. Start Timer
    tickCounter = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    timer->start(1500); // 1 tick = 1.5 seconds (updated in ms)


}

MainWindow::~MainWindow() {
    delete map;
    delete sim;
}

void MainWindow::updateSimulation() {
    sim->advanceTime();
    tickCounter++;

    // Spawn a new vehicle every 2 ticks (every 3 seconds)
    if (tickCounter % 2 == 0) {
        sim->spawnVehicle(0, 3, false); // Spawn from A to D
        if (tickCounter % 4 == 0) {
            sim->spawnVehicle(0, 3, true); // Spawn an emergency vehicle occasionally
        }
    }

    // Update the dashboard text
    int total = sim->getVehicleCount();
    statsLabel->setText(QString("Total Vehicles: %1 | (Emergency vehicles are RED)").arg(total));

    // Redraw everything
    scene->clear();
    drawMap();
    drawVehicles();
}

QPointF MainWindow::getNodePosition(int nodeId) {
    // These coordinates match the A, B, C, D, E layout in your screenshot
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
    QPen roadPen(Qt::gray, 8); // Thick roads

    // Draw roads
    scene->addLine(150, 150, 650, 150, roadPen); // A-B
    scene->addLine(150, 150, 150, 450, roadPen); // A-C
    scene->addLine(650, 150, 650, 450, roadPen); // B-D
    scene->addLine(150, 450, 650, 450, roadPen); // C-D
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

        // Add the letter label
        QGraphicsTextItem* text = scene->addText(QString(QChar('A' + i)), font);
        text->setDefaultTextColor(Qt::white);
        text->setPos(pos.x() - 10, pos.y() - 15);
    }
}

void MainWindow::drawVehicles() {
    for (int i = 0; i < sim->getVehicleCount(); i++) {
        int nodeId = sim->getVehicleLocation(i);

        if (nodeId != -1) {
            QPointF pos = getNodePosition(nodeId);

            // Properly check if this specific vehicle is an emergency
            QBrush vehicleBrush = sim->isEmergencyVehicle(i) ? QBrush(Qt::red) : QBrush(Qt::green);

            // Offset the cars slightly so a line of 3 cars doesn't look like 1 car
            int offsetX = (i % 3) * 8;
            int offsetY = (i % 3) * 8;

            scene->addEllipse(pos.x() - 8 + offsetX, pos.y() - 8 + offsetY, 16, 16, QPen(Qt::NoPen), vehicleBrush);
        }
    }
}