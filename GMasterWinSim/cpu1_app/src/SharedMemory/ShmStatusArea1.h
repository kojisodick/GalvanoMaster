/*
 * ShmStatusArea1.h
 *
 *  Created on: 2018/12/17
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHMSTATUSAREA1_H_
#define SRC_SHAREDMEMORY_SHMSTATUSAREA1_H_

#include "Common.h"

struct ShmStatusArea1 {
	volatile AppState cpu0State;
	AppState          cpu1State;
};

#endif /* SRC_SHAREDMEMORY_SHMSTATUSAREA1_H_ */
