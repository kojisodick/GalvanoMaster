#include "GMGraphicItemArc.h"

GMGraphicItemArc::GMGraphicItemArc(qreal x, qreal y, qreal width, qreal height, int id, int mode, QGraphicsItem *parent)
    : QGraphicsEllipseItem(x,y,width,height,parent)
{

    setData(100, mode);

    m_id = id;
    m_color = Qt::blue;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

#include <QPainter>
#include <QStyleOptionGraphicsItem>

void GMGraphicItemArc::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->save();

    QColor cl = (option->state & QStyle::State_Selected) ? m_color.darker(150) : m_color;
    if (option->state & QStyle::State_MouseOver)
        cl = cl.lighter(125);

    QPen pen_org = painter->pen();
    QPen pen = pen_org;

    //if (option->state & QStyle::State_MouseOver) {
    //    pen.setColor(Qt::blue);
    //}
    //else 
        if (option->state & QStyle::State_Selected) {
        pen.setColor(Qt::red);
    }
    painter->setPen(pen);

    QRectF rect = this->rect();
    painter->drawArc(rect,startAngle(),spanAngle());

    painter->restore();
}

void GMGraphicItemArc::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsEllipseItem::hoverEnterEvent(event);
}

void GMGraphicItemArc::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsEllipseItem::hoverLeaveEvent(event);
}