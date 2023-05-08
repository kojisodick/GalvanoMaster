/*
 * SharedMemory.h
 *
 *  Created on: 2018/12/17
 *      Author: imao_yusuke
 */

#ifndef __SHRAREDMEMORY1_H__
#define __SHRAREDMEMORY1_H__

#include "SharedMemory.h"

struct ShmCommand;

struct GatheringParameter
{
	uint1 item[16];
	uint4 period;
	uint4 bufNum;
	uint4 dataCounter;
};

struct SharedMemoryControlArea
{
	uint4 cpu0State;
	uint4 cpu1State;

	uint4 gatRequest;

};

class SharedMemory1 : public SharedMemory
{
public:
	SharedMemory1();

	int4 recvCmdRequest(ShmCommand &cmd)
	{
		return m_cmdArea->recvRequest(cmd);
	}

	int4 sendCmdResponse(const ShmCommand &cmd)
	{
		return m_cmdArea->sendResponse(cmd);
	}

};

#endif /* SRC_SHAREDMEMORY_SHAREDMEMORY_H_ */
