/*
 * ShmCommandArea.cpp
 *
 *  Created on: 2018/12/17
 *      Author: imao_yusuke
 */

#include "ShmCommandArea1.h"


int4 ShmCommandArea1::recvRequest(ShmCommand &cmd)
{
	if( m_state != Request ) {
		return ShmCmdErrNotRequested;
	}

	memcpy(&cmd, (const void*)&m_command, MAX_LEN_OF_SHM_CMD);

	m_state = Running;

	return ShmCmdErrNoError;
}

int4 ShmCommandArea1::sendResponse(const ShmCommand &cmd)
{
	if( m_state != Running ) {
		return ShmCmdErrInvalidState;
	}

	memcpy((void*)&m_command, &cmd, MAX_LEN_OF_SHM_CMD);

	m_state = Acknowledge;

	return ShmCmdErrNoError;
}
