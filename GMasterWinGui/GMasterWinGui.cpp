#include "GMasterWinGui.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QListWidget>
#include <QSplitter>
#include <QTabWidget>
#include <QToolButton>
#include <QToolBar>
#include <QStatusBar>
#include <QPushButton>

#include "GMGraphicArea.h"
#include "PlRegister.h"

GMasterWinGui::GMasterWinGui(QWidget *parent)
    : QMainWindow(parent)
{
    //ui.setupUi(this);

    m_simulationDone = false;
      
    m_tab = new QTabWidget();
    //QPushButton * btn = new QPushButton("Add new tab");
    //connect(btn, SIGNAL(clicked()), this, SLOT(createNewTab()));
    //m_tab->setCornerWidget(btn);

    createNewTab();

    m_lbl_status = new QLabel();

    QStatusBar * m_statusBar = new QStatusBar();
    m_statusBar->addWidget(m_lbl_status);
    this->setStatusBar(m_statusBar);

    this->setCentralWidget(m_tab);

	m_timerId = startTimer(200);

}

GMasterWinGui::~GMasterWinGui()
{}

void GMasterWinGui::createNewTab()
{
    m_curGraphic = new GMGraphicArea();
    m_list = new QListWidget();
    
    QSplitter * splitter = new QSplitter();
    splitter->setOrientation(Qt::Vertical);
    splitter->addWidget(m_curGraphic);
    splitter->addWidget(m_list);

    int count = m_tab->count();

    m_tab->addTab(splitter, QString("Result %1").arg(count+1));

}

void GMasterWinGui::timerEvent(QTimerEvent *event)
{
	PlRegister * pl = PlRegister::obj();

	if (event->timerId() == m_timerId) {
		uint4 pl_status = pl->getPlStatus();
		QString str_status = pl_status == PlStsBoot ? "Boot"
			: pl_status == PlStsInit ? "Init"
			: pl_status == PlStsWait ? "Wait"
			: pl_status == PlStsActive ? "Active"
			: pl_status == PlStsBusy ? "Busy"
			: pl_status == PlStsHalt ? "Halt"
			: pl_status == PlStsFormat ? "Format"
			: pl_status == PlStsError ? "Error"
			: "Unknown";

        m_lbl_status->setText(str_status);

        if (m_simulationDone) {
            createNewTab();
            m_simulationDone = false;
        }
	}
}

#include <QDateTime>

void GMasterWinGui::output(QString type, QString msg)
{
	
	//m_list->addItem(QString("%1 : %2 %3").arg(QDateTime::currentDateTime().toString()).arg(type).arg(msg));
	m_list->addItem(QString("%1 %2").arg(type).arg(msg));
}

void GMasterWinGui::moveTo(QPointF p, int blockmode)
{
    m_curGraphic->moveToLine(p.x(),p.y(), blockmode);
}

void GMasterWinGui::simulationDone()
{
    m_curGraphic->simulationDone();
    m_simulationDone = true;
}

void GMasterWinGui::simulationStart()
{
    m_tab->setCurrentIndex(m_tab->count()-1);
}