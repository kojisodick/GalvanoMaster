#pragma once
#include <qwidget.h>
#include <QMutex>
#include <QQueue>

class QGraphicsScene;
class QGraphicsItem;
class QTreeWidgetItem;
class QTreeWidget;
class View;

class GMDataType {
public:
    enum {GM_LINEAR = 0, GM_ARC}; 
    GMDataType(int blockmode, double in_x, double in_y) {
        mode = blockmode;
        type = GM_LINEAR;
        x = in_x;
        y = in_y;
        cx = 0;
        cy = 0;
        angle = 0;
    }
    GMDataType(int blockmode, double in_x, double in_y, double in_cx, double in_cy, double in_angle) {
        mode = blockmode;
        type = GM_ARC;
        x = in_x;
        y = in_y;
        cx = in_cx;
        cy = in_cy;
        angle = in_angle;
    }
    int mode;
    int type;
    double x, y;
    double cx, cy;
    double angle;
};

class GMGraphicArea : public QWidget
{
    Q_OBJECT
public:
    GMGraphicArea(QWidget * parent = 0);

    void moveToLine(double x, double y, int blockmode);
    void moveToArc(double x, double y, double center_x, double center_y, int blockmode);
    void simulationDone();

private slots:
    void itemSelectionChanged();
    void treeItemSelected(QTreeWidgetItem * item, int col);
    void modeSelectionChange();
    void dataPointChanged(int point);
private:
    int m_curPoint;
    double m_x_min;
    double m_x_max;
    double m_y_min;
    double m_y_max;
    View * m_view;
    QTreeWidget * m_tree;

    void timerEvent(QTimerEvent *event);

    QList< QGraphicsItem *> m_path;
    QList< QTreeWidgetItem *> m_items;
    QPointF m_p;

    QGraphicsScene * m_scene;
    QMutex m_mutex;
    QQueue<GMDataType> m_queue;
    int m_timerId;
    bool m_simulationDone;

};

