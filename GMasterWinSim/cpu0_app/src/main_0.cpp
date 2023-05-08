/*
 * Empty C++ Application
 */

#include <Logger/Fatfs.h>
#include <Logger/Uart.h>
#include "PL/PlRegister.h"
#include "SharedMemory0.h"
#include "Platform0.h"
#include "Server.h"
#include "Commands.h"
#include "ListBoxManager.h"

Logger *gp_log;
#ifdef __arm__
Uart  g_log(Logger::LevelDebug);
#else
Fatfs g_log(Logger::LevelDebug, "ps.log");
#endif

SharedMemory0  *gp_sharedMemory;
Platform0      *gp_platform;
Server         *gp_server;
Commands       *gp_gbCommand;
ListBoxManager *gp_listManager;

PlRegister     *gp_plReg;


#ifdef __arm__
int main()
#else
int main_0()
#endif
{
	gp_log          = &g_log;
	gp_sharedMemory = SharedMemory0::obj();
	gp_platform     = Platform0::obj();
	gp_server       = Server::obj();
	gp_gbCommand    = Commands::obj();
	gp_listManager  = ListBoxManager::obj();

	gp_plReg        = PlRegister::obj();
	

	gp_log->init();

	gp_gbCommand->init();
	gp_listManager->init();
	gp_sharedMemory->init();

	gp_log->info("<< Version Information >>");
	gp_log->info("BOOT         : %u.%u.%u.%u",
			PS_VERSION.major, BIN_VERSION.minor, BIN_VERSION.revision, BIN_VERSION.build);
	gp_log->info("PL           : 0x%x", gp_plReg->getPlVersion());
	gp_log->info("PS           : %u.%u.%u.%u",
			PS_VERSION.major, PS_VERSION.minor, PS_VERSION.revision, PS_VERSION.build);
	gp_log->info("Release Date : 20%u/%u/%u",
			RELEASE_DATE.year, RELEASE_DATE.month, RELEASE_DATE.day);
	gp_log->info(VERSION_COMMENT);

	gp_log->info("<< Application Log >>");
	gp_log->info("Start cpu0 application");
	gp_sharedMemory->setCpu0State(AppRunning);


	if( gp_sharedMemory->wakeupCpu1App() ) {
		gp_log->error("CPU1 Application startup failed.");
		return -1;
	}
	gp_log->info("Start cpu1 application");

	int err = gp_platform->init();

	if( err ){
		gp_log->error("Platform initialize error");
		return err;
	}

	err = gp_server->init();
	if( err ){
		gp_log->error("Server initialize error");
		return err;
	}

	gp_platform->enableInterrupt();

	while( 1 ) {
		err = gp_server->start();
		if( err ) {
			gp_log->error("Cannot start server");
			return err;
		}
	}

	// do not reach.
	gp_platform->cleanup();

	return 0;
}
