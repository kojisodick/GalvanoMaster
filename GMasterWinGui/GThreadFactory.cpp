#include "GThreadFactory.h"
#include <QThread>

extern int main_0();
extern int main_1();

GThreadFactory * GThreadFactory::obj()
{
	static GThreadFactory obj;
	return &obj;
}

typedef void(*EntryFunc)(void);

void GThreadFactory::entry(void * args)
{
	EntryFunc entry = (EntryFunc)args;
	entry();
}

void GThreadFactory::create()
{
	static int args[10];
	for (int i = 0; i < 10; i++) {
		args[i] = 777;
	}
	QThread * thread_0 = QThread::create(GThreadFactory::entry, (void *)main_0);
	thread_0->start();
    //thread_0->currentThreadId();

	//QThread * thread_1 = QThread::create(GThreadFactory::entry, (void *)main_1);
	//thread_1->start();

}