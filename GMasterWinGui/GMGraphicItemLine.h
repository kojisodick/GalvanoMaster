#pragma once

#include <QGraphicsLineItem>

class GMGraphicItemLine : public QGraphicsLineItem
{
public:
    GMGraphicItemLine(const QLineF &line, int id, int mode, QGraphicsItem *parent = nullptr);

    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    int id() { return m_id; }
    int type() const override { return QGraphicsItem::UserType;  }
    int mode() { return m_mode; }
private:
    QColor m_color;
    int m_id;
    int m_mode;

    //void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

};
