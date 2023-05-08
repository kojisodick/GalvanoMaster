#pragma once

#include <QMainWindow>
#include "ui_GMasterWinGui.h"

class QLabel;
class QListWidget;
class GMGraphicArea;
class QTabWidget;

class GMasterWinGui : public QMainWindow
{
    Q_OBJECT

public:
    GMasterWinGui(QWidget *parent = nullptr);
    ~GMasterWinGui();
	void output(QString type, QString msg);
    void moveTo(QPointF p, int blockmode);
    void simulationDone();
    void simulationStart();

protected:
	void timerEvent(QTimerEvent *event) override;

private:
	int m_timerId;
    QLabel * m_lbl_status;
    QListWidget * m_list;
    GMGraphicArea * m_curGraphic;
    QTabWidget * m_tab;
    bool m_simulationDone;
private slots:
    void createNewTab();

};
