/*
 * PlatformZynq.cpp
 *
 *  Created on: 2018/11/30
 *      Author: imao_yusuke
 */

#ifdef __arm__


#include "Platform.h"

#include "xparameters.h"
#include "xscugic.h"
#include "xtmrctr.h"
#include "xstatus.h"    //XST_SUCCESS
#include "xil_types.h"  //u8
#include "xil_cache.h"
#include "xil_mmu.h"

#include "Gathering.h"

#define OCM0_BASEADDR            (XPAR_PS7_RAM_0_S_AXI_BASEADDR)
#define OCM3_BASEADDR            (0xFFFF0000)
#define DISABLE_OCM_CACHE        (0x14de2)             // S=b1 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0


#define INTC_DEVICE_ID		     XPAR_SCUGIC_SINGLE_DEVICE_ID

#define FIQ_TMRCTR_DEVICE_ID	 XPAR_AXI_TIMER_FIQ_DEVICE_ID
//#define FIQ_ID			    	 XPAR_FABRIC_AXI_TIMER_FIQ_INTERRUPT_INTR
#define FIQ_ID			    	 (28)
#define FIQ_INTR_PRIO            (0x20)
#define FIQ_INTERVAL(n)          (XPAR_AXI_TIMER_FIQ_CLOCK_FREQ_HZ*0.001*n)  //msec
#define FIQ_RESET_VALUE(n)       FIQ_INTERVAL(n*0.001)
#define FIQ_LED_INTVL_CNT        (1000000/10) //1sec

#define INTR_TRIGGER_TYPE        (0x3)
#define TIMER_CNTR_0	 	     (0)


static void setupMmu();
static int4 setupInterrupt();
static void setupInterruptSystem();


static XScuGic g_interruptCtrl;
static XTmrCtr g_timerCntr;

extern Gathering *gp_gather;

static uint4 counter = 0;

void FIQ_Handler(void *CallBackRef, u8 TmrCtrVal)
{
	counter++;

	gp_gather->measure();
}


Platform::Platform()
{

}

int4 Platform::init(uint4 controlCycle)
{
	if(controlCycle !=5 && controlCycle != 10) {
//		gp_log->error("invalid control cycle(%u)\r\n", controlCycle);
		return -1;
	}

	setupMmu();

    int4 status = XTmrCtr_Initialize(&g_timerCntr, FIQ_TMRCTR_DEVICE_ID);

    if(status != XST_SUCCESS){
//    	gp_log->error("XTmrCtr_Initialize failed.\r\n");
		return -1;
	}

    status = XTmrCtr_SelfTest(&g_timerCntr, TIMER_CNTR_0);

    if(status != XST_SUCCESS){
//    	gp_log->error("XTmrCtr_SelfTest failed.\r\n");
		return -1;
	}

	XTmrCtr_SetHandler(&g_timerCntr, FIQ_Handler, &g_timerCntr);

	XTmrCtr_SetResetValue(&g_timerCntr, TIMER_CNTR_0, FIQ_RESET_VALUE(controlCycle));

	XTmrCtr_SetOptions(&g_timerCntr, TIMER_CNTR_0, XTC_INT_MODE_OPTION | XTC_AUTO_RELOAD_OPTION |XTC_DOWN_COUNT_OPTION);

	status = setupInterrupt();

    if(status != NoError){
//    	gp_log->error("Interrupt initialization failed.\r\n");
		return status;
	}

	XTmrCtr_Start(&g_timerCntr, TIMER_CNTR_0);

	return NoError;
}

void Platform::cleanup()
{
	Xil_ICacheDisable();

	Xil_DCacheDisable();
}


void setupMmu()
{
	//Xil_SetTlbAttributes(OCM0_BASEADDR, DISABLE_OCM_CACHE);

	Xil_SetTlbAttributes(OCM3_BASEADDR, DISABLE_OCM_CACHE);
}


int4 setupInterrupt()
{
	XScuGic_Config *GicConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);

	if(GicConfig == NULL){
//		gp_log->error("Not found XScuGic device.\r\n");
		return -1;
	}

	int4 status = XScuGic_CfgInitialize(&g_interruptCtrl, GicConfig,
			GicConfig->CpuBaseAddress);

	if(status != XST_SUCCESS) {
//		gp_log->error("XScuGic Initialization failed.\r\n");
		return -1;
	}

	XScuGic_SetPriorityTriggerType(&g_interruptCtrl, FIQ_ID, FIQ_INTR_PRIO, INTR_TRIGGER_TYPE);


	setupInterruptSystem();  // this function must be called before calling XScuGic_Connect


	status = XScuGic_Connect(&g_interruptCtrl, FIQ_ID,
			(Xil_ExceptionHandler) XTmrCtr_InterruptHandler, &g_timerCntr);

	if(status != XST_SUCCESS){
//		gp_log->error("XScuGic_Connect failed.\r\n");
		return -1;
	}

	XScuGic_Enable(&g_interruptCtrl, FIQ_ID);

	XScuGic_CPUWriteReg(&g_interruptCtrl, 0x0, 0xf);

	return NoError;
}


void setupInterruptSystem()
{
	Xil_ExceptionInit();

	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_FIQ_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler, &g_interruptCtrl);

	Xil_ExceptionEnableMask(XIL_EXCEPTION_ALL);
}





#endif
