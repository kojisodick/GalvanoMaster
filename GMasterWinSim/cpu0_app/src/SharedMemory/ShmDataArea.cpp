/*
 * ShmDataArea.cpp
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#include <Logger/Logger.h>
#include "ShmDataArea.h"

extern Logger *gp_log;

void ShmDataArea::init()
{
	m_state        = Waiting;
	m_writeIndex   = 0;
	m_readIndex    = 0;
	m_totalDataLen = 0;

	memset((void*)m_data, 0, SIZE_OF_DATA_BUF_IN_SHMEM);
}

int4 ShmDataArea::upload(const uint1 *dataAddr, const uint4 dataLen)
{
	this->init();

	uint4 timeoutCounter   = 0;
	uint4 remainingDataLen = dataLen;
	m_totalDataLen         = dataLen;

	m_state = Transferring;

	while( true ) {

		// Timeout Process
		if( m_writeIndex != m_readIndex ) {
			SLEEP_USEC(1);
			timeoutCounter++;

			if( timeoutCounter >= 1000 ) {
				m_state = TimeOut;
				return -1;
			}

			continue;
		}

		timeoutCounter = 0;

		if( remainingDataLen <= 0 ){
			break;
		}

		// transfer process
		if( remainingDataLen > SIZE_OF_DATA_BUF_IN_SHMEM ) {
			memcpy((void*)m_data, &dataAddr[m_writeIndex], SIZE_OF_DATA_BUF_IN_SHMEM);

			m_writeIndex     += SIZE_OF_DATA_BUF_IN_SHMEM;
			remainingDataLen -= SIZE_OF_DATA_BUF_IN_SHMEM;
		}
		else {
			memcpy((void*)m_data, &dataAddr[m_writeIndex], remainingDataLen);

			m_writeIndex     += remainingDataLen;
			remainingDataLen  = 0;
		}

	}

	m_state = Complete;

	return 0;
}


int4 ShmDataArea::download(uint1 *bufAddr, const uint4 bufLen)
{
	uint4 timeoutCounter   = 0;

	while( m_state != Transferring ) {
		SLEEP_USEC(10);
		timeoutCounter++;

		if( timeoutCounter >= 1000 ) {
			gp_log->error("[ShmDataArea::download] TimeOut! (sts=%u)",  m_state);
			m_state = TimeOut;
			return -1; //TODO:
		}
	}

	timeoutCounter = 0;

	if(  m_totalDataLen > bufLen ) {
		gp_log->error("buffer is too small.");
		return -1; // TODO: data is too big
	}

	uint4 remainingDataLen = m_totalDataLen;


	while( m_state == Transferring ) {
		if( m_writeIndex == m_readIndex ) {
			SLEEP_USEC(1);
			timeoutCounter++;

			if( timeoutCounter >= 1000 ) {
				m_state = TimeOut;
				gp_log->error("Wait status TimeOut");
				return -1; //TODO:
			}

			continue;
		}

		timeoutCounter = 0;

		if( remainingDataLen > SIZE_OF_DATA_BUF_IN_SHMEM ) {
			memcpy(&bufAddr[m_readIndex], (void*)m_data, SIZE_OF_DATA_BUF_IN_SHMEM);

			m_readIndex      += SIZE_OF_DATA_BUF_IN_SHMEM;
			remainingDataLen -= SIZE_OF_DATA_BUF_IN_SHMEM;
		}
		else {
			memcpy(&bufAddr[m_readIndex], (void*)m_data,  remainingDataLen);

			m_readIndex      += remainingDataLen;
			remainingDataLen  = 0;
		}
	}

	if( m_state !=  Complete ) {
		gp_log->error("not complete.");
		return -1; //TODO:
	}

	return 0;
}
