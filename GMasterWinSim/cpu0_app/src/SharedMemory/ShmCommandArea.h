/*
 * ShmCommandArea.h
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHMCOMMANDAREA_H_
#define SRC_SHAREDMEMORY_SHMCOMMANDAREA_H_

#include "ShmCommand.h"

class ShmCommandArea
{
public:
	void init();

	// for CPU0
	int4 sendRequest(const ShmCommand &cmd);
	int4 recvResponse(ShmCommand &cmd);

	// for CPU1
	int4 recvRequest(ShmCommand &cmd);
	int4 sendResponse(const ShmCommand &cmd);

#ifdef __arm__
private:
#endif
	ShmCommandArea(){}
	~ShmCommandArea() {}
private:

	enum State: uint4
	{
		Waiting     = 0,
		Request     = 1,
		Running     = 2,
		Acknowledge = 3,
		ErrorStop   = 4
	} volatile m_state;

	volatile ShmCommand m_command;
};

#endif /* SRC_SHAREDMEMORY_SHMCOMMANDAREA_H_ */
