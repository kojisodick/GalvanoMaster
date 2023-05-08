#include "GMGraphicArea.h"

#include <QGraphicsScene>
#include <QVBoxLayout>
#include <QTreeWidget>
#include <QSplitter>
#include <QTimerEvent>

#include "GMGraphicView.h"
#include "GMGraphicItemArc.h"
#include "GMGraphicItemLine.h"

#include <QHeaderView>
#include "PlDef.h"

GMGraphicArea::GMGraphicArea(QWidget * parent)
{
    m_curPoint = 0;

    m_x_min = DBL_MAX;
    m_x_max = -DBL_MAX;
    m_y_min = DBL_MAX;
    m_y_max = -DBL_MAX;

    m_simulationDone = false;
    m_timerId = -1;
    m_path.clear();
    m_p = QPointF(0, 0);
    
    m_items.clear();
    m_path.clear();

    m_scene = new QGraphicsScene();
    connect(m_scene, SIGNAL(selectionChanged()), this, SLOT(itemSelectionChanged()));

    //m_scene->addText("Hello, world!");

    //GMGraphicItemArc * arc = new GMGraphicItemArc(10,10,100,50);
    //arc->setStartAngle(10);
    //arc->setSpanAngle(16*210);
    //arc->setScale(2.5);
    //scene->addItem(arc);


    m_view = new View("Name");
    m_view->view()->setScene(m_scene);

    connect(m_view, SIGNAL(modeSelectionChanged()), this, SLOT(modeSelectionChange()));
    connect(m_view, SIGNAL(dataPointChanged(int)), this, SLOT(dataPointChanged(int)));

    m_tree = new QTreeWidget();
    m_tree->setColumnCount(2);
    m_tree->setHeaderLabels({"Item","Value"});
    connect(m_tree, SIGNAL(itemClicked(QTreeWidgetItem *, int)), this, SLOT(treeItemSelected(QTreeWidgetItem *, int)));


    QSplitter * splitter = new QSplitter();
    splitter->setOrientation(Qt::Horizontal);
    splitter->addWidget(m_tree);
    splitter->addWidget(m_view);


    QVBoxLayout * v = new QVBoxLayout();
    v->addWidget(splitter);
    this->setLayout(v);

    //double scale = 100.0;
    //moveToLine( 10 * scale,  10 * scale);
    //moveToLine(510 * scale,  10 * scale);
    //moveToLine(510 * scale, 510 * scale);
    //moveToLine( 10 * scale, 510 * scale);
    //moveToLine( 10 * scale,  10 * scale);

    m_timerId = startTimer(200);

}

void GMGraphicArea::modeSelectionChange()
{
    for (int i = 0; i < m_path.count(); i++) {
        QGraphicsItem * g = m_path[i];
        if (g->type() == QGraphicsItem::UserType) { // LINEAR
            GMGraphicItemLine * ii = (GMGraphicItemLine*)g;
            int id = ii->id();
            bool visible = m_curPoint > i;
            if (ii->mode() == MoveModeJump) visible &= m_view->modePathEnabled(1);
            else if (ii->mode() == MoveModeMark) visible &= m_view->modePathEnabled(0);
            else if (ii->mode() == MoveModeVsc) visible &= m_view->modePathEnabled(2);

            ii->setVisible(visible);
        }
    }
    update();
}

void GMGraphicArea::simulationDone()
{
    int count = m_queue.count();
    m_simulationDone = true;
    m_view->setDataCount(count);

    modeSelectionChange();

    dataPointChanged(count);
}

void GMGraphicArea::dataPointChanged(int point)
{
    m_curPoint = point;
    modeSelectionChange();
}

void GMGraphicArea::treeItemSelected(QTreeWidgetItem * item, int col)
{
    m_scene->clearSelection();

    bool ok = false;
    int id = item->data(0, 100).toInt(&ok);
    if (ok == true) {
        QGraphicsItem * gItem = m_path[id];

        gItem->setSelected(true);

        update();
    }
}

void GMGraphicArea::itemSelectionChanged()
{
    QList<QGraphicsItem *> items = m_scene->selectedItems();
    if (items.count() == 1) {
        QGraphicsItem * item = items[0];
        if (item->type() == QGraphicsItem::UserType) { // LINEAR
            GMGraphicItemLine * line = (GMGraphicItemLine*)item;
            int id = line->id();
            QTreeWidgetItem * item = m_items[id];
            item->setExpanded(true);
            item->setSelected(true);
            m_tree->setCurrentItem(item);

        }
        else { // ARC

        }
    }
}

void GMGraphicArea::moveToLine(double x, double y, int blockmode)
{
    m_mutex.lock();

    double scale = fmin(DEFAULT_CANVAS_WIDTH, DEFAULT_CANVAS_HEIGHT);

    GMDataType line(blockmode, x/ scale,y/ scale);
    m_queue.enqueue(line);
    m_curPoint = m_queue.count();

    m_mutex.unlock();
}

void GMGraphicArea::moveToArc(double x, double y, double center_x, double center_y, int blockmode)
{

}


void GMGraphicArea::timerEvent(QTimerEvent *event)
{
    if (event->timerId() == m_timerId) {

        m_mutex.lock();

        while (m_queue.count() > 0) {

            GMDataType d = m_queue.dequeue();

            if (d.x < m_x_min) m_x_min = d.x;
            if (d.x > m_x_max) m_x_max = d.x;
            if (d.y < m_y_min) m_y_min = d.y;
            if (d.y > m_y_max) m_y_max = d.y;

            QPointF p_st = m_p;
            QPointF p_ed(d.x, d.y);

            int block_type = 1;
            int id = m_path.count();
            GMGraphicItemLine * line = new GMGraphicItemLine(QLineF(p_st, p_ed),id, d.mode);

            QTreeWidgetItem * item = new QTreeWidgetItem(m_tree);
            item->setText(0, QString("[%1]").arg(id));
            item->setData(0, 100, id);

            QTreeWidgetItem * item3 = new QTreeWidgetItem(item);
            item3->setText(0, "mode");
            item3->setText(1, QString::number(d.mode));
            QTreeWidgetItem * item1 = new QTreeWidgetItem(item);
            item1->setText(0, "cmdPos");
            item1->setText(1, QString("[%1,%2]").arg(d.x).arg(d.y));

            m_tree->addTopLevelItem(item);
            m_items.append(item);
            m_path.append(line);
            m_scene->addItem(line);

            m_p = p_ed;

            double x_span = m_x_max - m_x_min;
            double y_span = m_y_max - m_y_min;
            double span = fmax(x_span, y_span);

            m_view->setRange(m_x_min-x_span*0.1,m_x_max + x_span * 0.1, m_y_min - y_span * 0.1, m_y_max + y_span * 0.1);
                       
        }

        m_mutex.unlock();

        if (m_simulationDone == true && m_queue.count() == 0) {
            killTimer(m_timerId);
        }
    }
}