#include "mainwindow.h"
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsSimpleTextItem>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QStatusBar>
#include <QPainter>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle("Traffic Management Simulator — Day 2: Static Map");
    resize(900, 700);

    m_scene = new QGraphicsScene(this);
    m_scene->setSceneRect(0, 0, 900, 700);
    m_scene->setBackgroundBrush(QColor(25, 28, 35));

    m_view = new QGraphicsView(m_scene, this);
    m_view->setRenderHint(QPainter::Antialiasing, true);
    m_view->setDragMode(QGraphicsView::ScrollHandDrag);
    setCentralWidget(m_view);

    buildScene();
    statusBar()->showMessage("Day 2 — 5 intersections, 5 roads (static)");
}

void MainWindow::buildScene()
{
    addRoad(150, 150, 750, 150);
    addRoad(150, 150, 150, 550);
    addRoad(750, 150, 750, 550);
    addRoad(150, 550, 750, 550);
    addRoad(150, 150, 750, 550);

    addIntersection(150, 150, "A");
    addIntersection(750, 150, "B");
    addIntersection(150, 550, "C");
    addIntersection(750, 550, "D");
    addIntersection(450, 350, "E");
}

void MainWindow::addIntersection(qreal x, qreal y, const QString& label)
{
    const qreal radius = 32.0;
    QGraphicsEllipseItem* circle = m_scene->addEllipse(
        x - radius, y - radius, 2 * radius, 2 * radius,
        QPen(QColor(0, 200, 255), 3), QBrush(QColor(35, 75, 115)));
    circle->setZValue(10);

    QGraphicsSimpleTextItem* text = m_scene->addSimpleText(label);
    QFont f = text->font();
    f.setBold(true);
    f.setPointSize(14);
    text->setFont(f);
    text->setBrush(Qt::white);

    const QRectF br = text->boundingRect();
    text->setPos(x - br.width() / 2.0, y - br.height() / 2.0);
    text->setZValue(11);
}

void MainWindow::addRoad(qreal x1, qreal y1, qreal x2, qreal y2)
{
    QPen pen(QColor(200, 200, 200), 20);
    pen.setCapStyle(Qt::RoundCap);
    QGraphicsLineItem* line = m_scene->addLine(x1, y1, x2, y2, pen);
    line->setZValue(0);
}
