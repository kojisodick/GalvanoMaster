/*
 * SharedMemory.cpp
 *
 *  Created on: 2018/12/17
 *      Author: imao_yusuke
 */

#include "SharedMemory1.h"



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

SharedMemory::SharedMemory()
: m_cmdArea ((ShmCommandArea*)(SHAREDMEM_BASEADDR + SHM_CMD_AREA_OFFSET )),
  m_stsArea ((ShmStatusArea* )(SHAREDMEM_BASEADDR + SHM_STS_AREA_OFFSET )),
  m_dataArea((ShmDataArea*   )(SHAREDMEM_BASEADDR + SHM_DATA_AREA_OFFSET))
{
	Xil_SetTlbAttributes(PS7_RAM_0_BASEADDR, DISABLE_OCM_CACHE);
	Xil_SetTlbAttributes(PS7_RAM_1_BASEADDR, DISABLE_OCM_CACHE);
}

#else
SharedMemory1::SharedMemory1()
	: SharedMemory()
{
}
#endif

