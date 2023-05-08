#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

#include "ShmCommandArea.h"
#include "ShmStatusArea.h"
#include "ShmDataArea.h"


class SharedMemory
{
public:
	SharedMemory();

	void setCpu0State(AppState state) { m_stsArea->cpu0State = state; }
	AppState getCpu0State() { return m_stsArea->cpu0State; }
	
	void setCpu1State(AppState state) { m_stsArea->cpu1State = state; }
	AppState getCpu1State() { return m_stsArea->cpu1State; }

	int4 uploadData(const uint1 *dataAddr, uint4 dataLen){
		return m_dataArea->upload(dataAddr, dataLen);
	}

	int4 downloadData(uint1 *bufAddr, uint4 bufLen){
		return m_dataArea->download(bufAddr, bufLen);
	}

	uint4 getTotalDataLen(){
		return m_dataArea->getTotalDataLen();
	}

protected:
	ShmCommandArea * m_cmdArea;
	ShmStatusArea  * m_stsArea;
	ShmDataArea    * m_dataArea;

};


#endif
