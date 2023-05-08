/*
 * ShmDataArea.h
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHMDATAAREA_H_
#define SRC_SHAREDMEMORY_SHMDATAAREA_H_

#include "Common.h"

#define SIZE_OF_DATA_BUF_IN_SHMEM    (0x1000)

class ShmDataArea
{
public:
	void init();

	int4 upload(const uint1 *dataAddr, uint4 dataLen);
	int4 download(uint1 *bufAddr, uint4 bufLen);

	uint4 getTotalDataLen(){ return m_totalDataLen; }

#ifdef __arm__
private:
#endif
	ShmDataArea(){}
	~ShmDataArea() {}
private:

	enum State :uint4
	{
		Waiting,
		Transferring,
		Complete,
		TimeOut,
	} volatile m_state;

	volatile uint4 m_writeIndex;
	volatile uint4 m_readIndex;
	volatile uint4 m_totalDataLen;
	volatile uint1 m_data[SIZE_OF_DATA_BUF_IN_SHMEM];

};

#endif /* SRC_SHAREDMEMORY_SHMDATAAREA_H_ */
