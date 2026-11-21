#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QGraphicsScene;
class QGraphicsView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override = default;

private:
    void buildScene();
    void addIntersection(qreal x, qreal y, const QString& label);
    void addRoad(qreal x1, qreal y1, qreal x2, qreal y2);

    QGraphicsScene* m_scene = nullptr;
    QGraphicsView*  m_view  = nullptr;
};

#endif // MAINWINDOW_H
