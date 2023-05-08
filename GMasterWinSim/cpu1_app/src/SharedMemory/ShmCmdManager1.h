/*
 * ShmCmdManager.h
 *
 *  Created on: 2018/12/21
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHMCMDMANAGER_H_
#define SRC_SHAREDMEMORY_SHMCMDMANAGER_H_

#include "SharedMemory1.h"
#include "ShmCommand.h"

class ShmCmdManager1
{
public:
	ShmCmdManager1();
	~ShmCmdManager1(){}

	void init();

	void runCommand();

private:
	SharedMemory1 *m_sharedMem;
	ShmCommand    m_command;

	int4 (*m_funcPtr[MaxNumOfShmCmd])(SCmdArgs &args);
};

#endif /* SRC_SHAREDMEMORY_SHMCMDMANAGER_H_ */
