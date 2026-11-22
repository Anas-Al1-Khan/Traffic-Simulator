#include <QGraphicsDropShadowEffect>
#include "vehicleitem.h"
#include <QPen>
#include <QBrush>
VehicleItem::VehicleItem(int id, double startX, double startY, double endX, double endY, bool isEmergency)
    : QObject(nullptr), QGraphicsEllipseItem(-6, -6, 12, 12), m_startX(startX), m_startY(startY), m_endX(endX), m_endY(endY), m_progress(0.0), m_isEmergency(isEmergency), m_flashState(false)
{
Q_UNUSED(id);
    // --- UI UPGRADE: Drop Shadow ---
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(10);
    shadow->setColor(QColor(0, 0, 0, 200));
    shadow->setOffset(2, 2);
    setGraphicsEffect(shadow);
    setPos(m_startX, m_startY);
    setZValue(20);
    if (m_isEmergency) {
        setBrush(QBrush(Qt::blue));
        setPen(QPen(Qt::cyan, 2));
        m_flashTimer = new QTimer(this);
        connect(m_flashTimer, &QTimer::timeout, this, &VehicleItem::toggleFlash);
        m_flashTimer->start(200);
    } else {
        setBrush(QBrush(Qt::yellow));
        setPen(QPen(Qt::black, 1));
        m_flashTimer = nullptr;
    }
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &VehicleItem::moveStep);
}
void VehicleItem::startMoving() { m_timer->start(16); }
void VehicleItem::moveStep() {
    m_progress += m_isEmergency ? 0.02 : 0.01;
    if (m_progress >= 1.0) {
        m_progress = 1.0;
        m_timer->stop();
        if (m_flashTimer) m_flashTimer->stop();
    }
    updatePosition();
}
void VehicleItem::updatePosition() {
    setPos(m_startX + (m_endX - m_startX) * m_progress, m_startY + (m_endY - m_startY) * m_progress);
}
void VehicleItem::toggleFlash() {
    m_flashState = !m_flashState;
    if (m_flashState) { setBrush(QBrush(Qt::cyan)); setPen(QPen(Qt::white, 2)); }
    else { setBrush(QBrush(Qt::blue)); setPen(QPen(Qt::cyan, 2)); }
}
