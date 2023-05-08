#pragma once

#include <QGraphicsEllipseItem>

class GMGraphicItemArc : public QGraphicsEllipseItem
{
public:

    GMGraphicItemArc(qreal x, qreal y, qreal width, qreal height, int id, int mode, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    int id() { return m_id; }
    int type() const override { return QGraphicsItem::UserType + 1; }

private:
    QColor m_color;
    int m_id;

    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

};

