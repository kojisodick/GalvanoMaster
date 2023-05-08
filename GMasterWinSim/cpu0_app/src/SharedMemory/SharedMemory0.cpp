/*
 * SharedMemory.cpp
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#include "SharedMemory0.h"

#define GET_STS_INTERVAL        (100) //usec
#define WAKEUP_TIMEOUT_LIMIT    (10*1000*1000/GET_STS_INTERVAL) //10sec

SharedMemory0 * SharedMemory0::obj()
{
	static SharedMemory0 obj;
	return &obj;
}

#ifdef __arm__

#include "xparameters.h"
#include "xil_mmu.h"

// NOTE: These address is defined in the linker script
#define PS7_RAM_0_BASEADDR      (0x00000000) // OCM_0
#define PS7_RAM_1_BASEADDR      (0xFFFF0000) // OCM_3

#define DISABLE_OCM_CACHE       (0x14de2)    // S=b1 TEX=b100 AP=b11, Domain=b1111, C=b0, B=b0

#define SHAREDMEM_BASEADDR      PS7_RAM_0_BASEADDR

#define SHM_CMD_AREA_OFFSET     (0x00000000)
#define SHM_STS_AREA_OFFSET     (0x00000100)
#define SHM_DATA_AREA_OFFSET    (0x00001000)

#define SIZE_OF_SHM_CMD_AREA    (0x00000100)
#define SIZE_OF_SHM_STS_AREA    (0x00000100)
#define SIZE_OF_SHM_DATA_AREA   (0x00040000)

#define sev() __asm__("sev")
#define CPU1APP_BASEADDR        (0x30000000) // NOTE: This address is defined in the linker script
#define CPU1STARTADR 		    (0xfffffff0)



SharedMemory0::SharedMemory0()
: m_cmdArea ((ShmCommandArea*)(SHAREDMEM_BASEADDR + SHM_CMD_AREA_OFFSET )),
  m_stsArea ((ShmStatusArea* )(SHAREDMEM_BASEADDR + SHM_STS_AREA_OFFSET )),
  m_dataArea((ShmDataArea*   )(SHAREDMEM_BASEADDR + SHM_DATA_AREA_OFFSET))
{
	Xil_SetTlbAttributes(PS7_RAM_0_BASEADDR, DISABLE_OCM_CACHE);
	Xil_SetTlbAttributes(PS7_RAM_1_BASEADDR, DISABLE_OCM_CACHE);
}


uint4 SharedMemory0::wakeupCpu1App()
{
	Xil_Out32(CPU1STARTADR, CPU1APP_BASEADDR);
    dmb(); //this function waits until writing is complete
    sev();

    uint4 counter = 0;
	while( m_stsArea->cpu1State != AppRunning ) {
		usleep(GET_STS_INTERVAL);
		++counter;

		if(counter >= WAKEUP_TIMEOUT_LIMIT ){
			return -1;
		}
	}
	return 0;
}


#else


SharedMemory0::SharedMemory0()
	: SharedMemory()
{
}


uint4 SharedMemory0::wakeupCpu1App()
{
    // nothing to do for Windows
	return 0;
}

#endif //__arm__

void SharedMemory0::init()
{
	m_cmdArea->init();
	m_stsArea->init();
	m_dataArea->init();
}

int4 SharedMemory0::runCommand(ShmCommand &cmd)
{
	m_cmdArea->sendRequest(cmd);
	return m_cmdArea->recvResponse(cmd);
}