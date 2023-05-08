/*
 * Empty C++ Application
 */


#include "Platform1.h"
#include "SharedMemory1.h"
#include "ShmCmdManager1.h"
#include "Dpram.h"
#include "Gathering1.h"

#ifdef __arm__
#include "xparameters.h"
#else
#define XPAR_BRAM_7_BASEADDR 0
#endif

Platform1     *gp_platform;
SharedMemory1 *gp_sharedMem,     g_sharedMem;
Gathering1    *gp_gather,        g_gather;
ShmCmdManager1*gp_shmCmdManager, g_shmCmdManager;
Dpram         *gp_dpram;

#ifdef __arm__
int main()
#else
int main_1()
#endif
{
	gp_platform      = Platform1::obj();
	gp_sharedMem     = &g_sharedMem;
	gp_gather        = &g_gather;
	gp_shmCmdManager = &g_shmCmdManager;
#ifdef __arm__
	gp_dpram         = (Dpram *)XPAR_BRAM_7_BASEADDR;
#else
	static Dpram dpram;
	gp_dpram = &dpram;
#endif
	AppState state;
	while( (state = gp_sharedMem->getCpu0State()) != AppRunning ) {
		SLEEP_USEC(10);
	}

	gp_sharedMem->setCpu1State(AppRunning);

	SLEEP_USEC(100);
	gp_dpram->init();

	int4 err = gp_platform->init(gp_dpram->getControlCycle());
	if( err != NoError ) {
		return err;
	}

	while( true ) {
		gp_shmCmdManager->runCommand();
		SLEEP_USEC(1);
	}

	return 0;
}
