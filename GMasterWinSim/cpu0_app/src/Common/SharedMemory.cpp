
#include "SharedMemory.h"

SharedMemory::SharedMemory()
{
#ifndef __arm__
	static ShmCommandArea objCmd;
	m_cmdArea = &objCmd;

	static ShmStatusArea  objSts;
	m_stsArea = &objSts;

	static ShmDataArea objArea;
	m_dataArea = &objArea;
#else
	m_cmdArea =(ShmCommandArea*)(SHAREDMEM_BASEADDR + SHM_CMD_AREA_OFFSET);
	m_stsArea =(ShmStatusArea*)(SHAREDMEM_BASEADDR + SHM_STS_AREA_OFFSET);
	m_dataArea = (ShmDataArea*)(SHAREDMEM_BASEADDR + SHM_DATA_AREA_OFFSET);
#endif

}