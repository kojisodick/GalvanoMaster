#include "GMasterWinGui.h"
#include <QtWidgets/QApplication>

#include "GThreadFactory.h"
GMasterWinGui * mainWidget = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GMasterWinGui w;
    mainWidget = &w;
    w.show();

	GThreadFactory::obj()->create();

    return a.exec();
}


void output_func(const char * type, const char * msg)
{
	if (mainWidget) {
        mainWidget->output(type,msg);
	}
}

void gui_move2line(double x, double y, int blockmode)
{
    if (mainWidget) {
        mainWidget->moveTo(QPointF(x,y), blockmode);
    }
}

void finish_simulation()
{
    if (mainWidget) {
        mainWidget->simulationDone();
    }
}

void start_new_run()
{
    if (mainWidget) {
        mainWidget->simulationStart();
    }
}