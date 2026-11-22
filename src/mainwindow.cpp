#include "mainwindow.h"
#include <QPen>
#include <QBrush>
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QLabel>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    // 1. Setup the Main Layout
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 10); // Tweak margins for a cleaner look

    // 2. Create Dashboard Label (HUD Style)
    statsLabel = new QLabel("Dashboard loading...", this);
    statsLabel->setStyleSheet(
        "color: #00e6e6;"            // Neon cyan
        "font-size: 18px;"
        "font-weight: bold;"
        "font-family: 'Segoe UI', Arial, sans-serif;"
        "padding: 15px;"
        "background-color: #1a1a24;"
        "border-bottom: 2px solid #00e6e6;"
    );
    layout->addWidget(statsLabel);

    // 3. Setup the Visual Scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    scene->setBackgroundBrush(QColor(15, 15, 25)); // Dark midnight blue

    view = new QGraphicsView(scene, this);
    view->setRenderHint(QPainter::Antialiasing, true); // Smooth lines
    layout->addWidget(view);

    // 4. Add Close Road Button (Styled)
    QPushButton* closeRoadBtn = new QPushButton("TOGGLE ROAD C-D", this);
    closeRoadBtn->setStyleSheet(
        "QPushButton {"
        "   background-color: #d90000;"
        "   color: white;"
        "   font-weight: bold;"
        "   font-size: 14px;"
        "   border-radius: 5px;"
        "   padding: 12px;"
        "   border: 2px solid #ff4d4d;"
        "   margin: 0 20px;"
        "}"
        "QPushButton:hover { background-color: #ff1a1a; }"
        "QPushButton:pressed { background-color: #990000; }"
    );
    layout->addWidget(closeRoadBtn);
    connect(closeRoadBtn, &QPushButton::clicked, this, &MainWindow::onCloseRoadClicked);

    isCDRoadClosed = false; // Initialize flag
    setCentralWidget(centralWidget);
    setWindowTitle("Traffic Management Simulator - Final Prototype");

    // 5. Setup Logic (Teammate's Graph and Simulation)
    map = new Graph(5);
    map->addRoad(0, 1, 4); // A - B
    map->addRoad(0, 2, 2); // A - C
    map->addRoad(1, 3, 5); // B - D
    map->addRoad(2, 3, 1); // C - D
    map->addRoad(0, 4, 3); // A - E
    map->addRoad(4, 3, 3); // E - D

    sim = new Simulation(map, 20);

    // Spawn initial cars
    sim->spawnVehicle(0, 3, false);
    sim->spawnVehicle(0, 3, false);
    sim->spawnVehicle(0, 3, true); // Emergency

    // 6. Draw initial state
    drawMap();

    // Initialize tracking variables for animation
    for (int i = 0; i < sim->getVehicleCount(); i++) {
        m_lastLocations.push_back(sim->getVehicleLocation(i));
    }

    // 7. Start Timer
    tickCounter = 0;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    timer->start(1500); // 1.5 seconds per tick
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
        m_lastLocations.push_back(-2); // -2 means newly spawned
        
        if (tickCounter % 4 == 0) {
            sim->spawnVehicle(0, 3, true); 
            m_lastLocations.push_back(-2);
        }
    }

    // Update the dashboard text
    statsLabel->setText(QString("TOTAL VEHICLES: %1  |  COMPLETED TRIPS: %2  |  (EMERGENCY = FLASHING BLUE)")
                        .arg(sim->getVehicleCount())
                        .arg(sim->getCompletedTrips()));

    // --- ANIMATION BRIDGE ---
    // Check if any cars moved to a new node, and spawn a smooth animation for them
    for (int i = 0; i < sim->getVehicleCount(); i++) {
        int currentNode = sim->getVehicleLocation(i);
        int lastNode = m_lastLocations[i];

        if (currentNode != -1 && lastNode != -1 && currentNode != lastNode) {
            QPointF startPos = getNodePosition(lastNode);
            QPointF endPos = getNodePosition(currentNode);
            bool isEmergency = sim->isEmergencyVehicle(i);

            // Spawn the animated car
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
    // 1. Draw Background Grid (Cyberpunk style)
    QPen gridPen(QColor(35, 40, 50), 1);
    for (int i = 0; i < 800; i += 40) {
        scene->addLine(i, 0, i, 600, gridPen);
    }
    for (int i = 0; i < 600; i += 40) {
        scene->addLine(0, i, 800, i, gridPen);
    }

    // 2. Draw Roads (Thick asphalt with dashed center lines)
    QPen roadPen(QColor(50, 50, 50), 16); 
    QPen lanePen(Qt::white, 2, Qt::DashLine);

    auto drawRoad = [&](int x1, int y1, int x2, int y2, bool isClosed) {
        QPen p = isClosed ? QPen(Qt::red, 16) : roadPen;
        scene->addLine(x1, y1, x2, y2, p);
        
        if (!isClosed) {
            scene->addLine(x1, y1, x2, y2, lanePen); // White dashed lane line
        }
    };

    drawRoad(150, 150, 650, 150, false);           // A-B
    drawRoad(150, 150, 150, 450, false);           // A-C
    drawRoad(650, 150, 650, 450, false);           // B-D
    drawRoad(150, 450, 650, 450, isCDRoadClosed);  // C-D (Dynamic)
    drawRoad(150, 150, 400, 300, false);           // A-E
    drawRoad(400, 300, 650, 450, false);           // E-D

    // 3. Draw Intersections (Neon Glow + Node + Text)
    QPen nodePen(Qt::cyan, 3);
    QBrush nodeBrush(Qt::darkBlue);
    QFont font = scene->font();
    font.setPointSize(16);
    font.setBold(true);

    for (int i = 0; i < 5; i++) {
        QPointF pos = getNodePosition(i);
        
        // Outer glow
        QGraphicsEllipseItem* glow = scene->addEllipse(pos.x() - 30, pos.y() - 30, 60, 60, QPen(Qt::NoPen), QBrush(QColor(0, 200, 255, 40)));
        glow->setZValue(1);
        
        // Main node circle
        QGraphicsEllipseItem* node = scene->addEllipse(pos.x() - 25, pos.y() - 25, 50, 50, nodePen, nodeBrush);
        node->setZValue(2);
        
        // Text Label
        QGraphicsTextItem* text = scene->addText(QString(QChar('A' + i)), font);
        text->setDefaultTextColor(Qt::white);
        text->setPos(pos.x() - 10, pos.y() - 15);
        text->setZValue(3);
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