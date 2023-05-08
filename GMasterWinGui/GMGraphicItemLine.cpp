#include "GMGraphicItemLine.h"

GMGraphicItemLine::GMGraphicItemLine(const QLineF &line, int id, int mode, QGraphicsItem *parent)
    : QGraphicsLineItem(line,parent)
{
    
    setData(100, mode);

    m_mode = mode;
    m_color = Qt::blue;
    m_id = id;

    setFlags(ItemIsSelectable);
    setAcceptHoverEvents(true);
}

#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include "PlDef.h"

extern bool gMode[3];

void GMGraphicItemLine::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget);
    painter->save();

    QColor cl = (option->state & QStyle::State_Selected) ? m_color.darker(150) : m_color;
    if (option->state & QStyle::State_MouseOver)
        cl = cl.lighter(125);

    QPen pen_org = painter->pen();
    QPen pen = pen_org;
    pen.setWidthF(0.2);

    if (m_mode == 1) pen.setColor(Qt::blue);
    else if( m_mode == 16 ) pen.setColor(Qt::green);
    else pen.setColor(Qt::black);

    //if (option->state & QStyle::State_MouseOver) {
    //    pen.setColor(Qt::blue);
    //}
    //else 
    if (option->state & QStyle::State_Selected) {
        pen.setColor(Qt::red);
    }
    painter->setPen(pen);

    painter->drawLine(line());

    painter->restore();
}

void GMGraphicItemLine::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsLineItem::hoverEnterEvent(event);
}

void GMGraphicItemLine::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    QGraphicsLineItem::hoverLeaveEvent(event);
}