/*
 * SharedMemory.h
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHAREDMEMORY_H_
#define SRC_SHAREDMEMORY_SHAREDMEMORY_H_

#include "SharedMemory.h"

#include "ShmCommandArea.h"
#include "ShmStatusArea.h"
#include "ShmDataArea.h"

class SharedMemory0 : public SharedMemory
{
private:
	SharedMemory0();

public:
	~SharedMemory0(){}

	static SharedMemory0 * obj();

	void init();

	uint4 wakeupCpu1App();

	int4 runCommand(ShmCommand &cmd);

	int4 sendCmdRequest(const ShmCommand &cmd)
	{
		return m_cmdArea->sendRequest(cmd);
	}

	int4 recvCmdResponse(ShmCommand &cmd)
	{
		return m_cmdArea->recvResponse(cmd);
	}


};

#endif /* SRC_SHAREDMEMORY_SHAREDMEMORY_H_ */
