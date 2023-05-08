/*
 * ShmStatusArea.h
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHMSTATUSAREA_H_
#define SRC_SHAREDMEMORY_SHMSTATUSAREA_H_

#include "Common.h"

struct ShmStatusArea
{
	volatile AppState cpu0State;
	volatile AppState cpu1State;

	void init()
	{
		cpu0State = AppWaiting;
		cpu1State = AppWaiting;
	}
};

#endif /* SRC_SHAREDMEMORY_SHMSTATUSAREA_H_ */
