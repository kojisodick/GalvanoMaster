/*
 * PlatformZynq.cpp
 *
 *  Created on: 2018/11/30
 *      Author: imao_yusuke
 */

#ifdef __arm__

#include "Platform.h"

#include "xil_cache.h"
#include "xil_exception.h"
#include "xparameters.h"
#include "xscugic_hw.h"
#include "xscutimer.h"

#include <Logger/Logger.h>
extern Logger *gp_log;

#define TIMER_DEVICE_ID         XPAR_SCUTIMER_DEVICE_ID
#define TIMER_IRPT_INTR         XPAR_SCUTIMER_INTR

#define INTC_DEVICE_ID          XPAR_SCUGIC_SINGLE_DEVICE_ID
#define INTC_BASE_ADDR          XPAR_SCUGIC_0_CPU_BASEADDR
#define INTC_DIST_BASE_ADDR     XPAR_SCUGIC_0_DIST_BASEADDR

#define RESET_RX_CNTR_LIMIT     (40)

volatile uint4 g_tcpFastTimerFlag = 0;
volatile uint4 g_tcpSlowTimerFlag = 0;
volatile uint4 g_RxDResetFlag     = 0;
volatile uint4 g_serverWatchDogCntr  = 0;

static XScuTimer scutimer;

void timer_callback(XScuTimer *timer)
{
	static uint4 odd = 1;
	static uint4 resetRxCntr     = 0;

	g_tcpFastTimerFlag = 1;

	odd = !odd;

	if( odd ){
		g_tcpSlowTimerFlag = 1;
	}

	resetRxCntr++;
	g_serverWatchDogCntr++;

	if( resetRxCntr >= RESET_RX_CNTR_LIMIT ){
		g_RxDResetFlag = 1;
		resetRxCntr = 0;
	}

	XScuTimer_ClearInterruptStatus(timer);
}

static int4 setupTimer();
static void setupInterrupts();


//---------------------------------------------------------------

Platform::Platform()
{

}

int4 Platform::init()
{
	gp_log->info("[Platform::init] Start.");

	if( setupTimer() ){
		return SCUTimerSetupFailed;
	}

	setupInterrupts();

	return NoError;
}


void Platform::cleanup()
{
	Xil_ICacheDisable();
	Xil_DCacheDisable();
}


void Platform::enableInterrupt()
{
	Xil_ExceptionEnableMask(XIL_EXCEPTION_IRQ);

	XScuTimer_EnableInterrupt(&scutimer);

	XScuTimer_Start(&scutimer);
}


void Platform::disableInterrupt()
{
	Xil_ExceptionDisableMask(XIL_EXCEPTION_IRQ);

	XScuTimer_DisableInterrupt(&scutimer);

	XScuTimer_Stop(&scutimer);

}


int4 setupTimer()
{
	gp_log->info("[Platform::setupTimer] Start.");

	XScuTimer_Config *ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);

//	gp_log->info("XScuTimer_LookupConfig() Called.");

	int4 res = XScuTimer_CfgInitialize(&scutimer, ConfigPtr, ConfigPtr->BaseAddr);

//	gp_log->info("XScuTimer_CfgInitialize() Called.");

	if( res != XST_SUCCESS ) {
		gp_log->error("[Platform::setupTimer] In %s: Scutimer Cfg initialization failed...", __func__);
		return -1;
	}

	res = XScuTimer_SelfTest(&scutimer);
	if( res != XST_SUCCESS ) {
		gp_log->error("[Platform::setupTimer] In %s: Scutimer Self test failed...", __func__);
		return -2;
	}

//	gp_log->info("XScuTimer_SelfTest() Called.");

	XScuTimer_EnableAutoReload(&scutimer);

//	gp_log->info("XScuTimer_EnableAutoReload() Called.");

	int4 TimerLoadValue = (int4)(XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ * 0.125);
	XScuTimer_LoadTimer(&scutimer, TimerLoadValue);

//	gp_log->info("XScuTimer_LoadTimer() Called.");

	return 0;
}


void setupInterrupts()
{
	gp_log->info("[Platform::setupInterrupts] Start.");

	Xil_ExceptionInit();

//	gp_log->info("[Platform::setupInterrupts] Xil_ExceptionInit() Called.");

	XScuGic_DeviceInitialize(INTC_DEVICE_ID);

//	gp_log->info("[Platform::setupInterrupts] XScuGic_DeviceInitialize() Called.");

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
			(Xil_ExceptionHandler)XScuGic_DeviceInterruptHandler,
			(void *)INTC_DEVICE_ID);

//	gp_log->info("[Platform::setupInterrupts] Xil_ExceptionRegisterHandler() Called.");

	XScuGic_RegisterHandler(INTC_BASE_ADDR,	TIMER_IRPT_INTR,
			(Xil_ExceptionHandler)timer_callback,
			(void *)&scutimer);

//	gp_log->info("[Platform::setupInterrupts] XScuGic_RegisterHandler() Called.");

	XScuGic_EnableIntr(INTC_DIST_BASE_ADDR, TIMER_IRPT_INTR);

//	gp_log->info("[Platform::setupInterrupts] XScuGic_EnableIntr() Called.");
}


#endif
