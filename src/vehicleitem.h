#ifndef VEHICLEITEM_H
#define VEHICLEITEM_H
#include <QObject>
#include <QGraphicsEllipseItem>
#include <QTimer>
class VehicleItem : public QObject, public QGraphicsEllipseItem
{
    Q_OBJECT
public:
    VehicleItem(int id, double startX, double startY, double endX, double endY, bool isEmergency);
    void startMoving();
private slots:
    void moveStep();
    void toggleFlash();
private:
    double m_startX, m_startY, m_endX, m_endY, m_progress;
    QTimer* m_timer;
    bool m_isEmergency;
    QTimer* m_flashTimer;
    bool m_flashState;
    void updatePosition();
};
#endif
