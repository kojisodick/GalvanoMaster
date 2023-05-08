/*
 * ShmCommandArea.cpp
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#include "ShmCommandArea.h"

void ShmCommandArea::init()
{
	m_state         = Waiting;
	m_command.code  = ShmCmdCodeInvalid;
	m_command.error = ShmCmdErrNoError;

	memset((void *)m_command.resurve,  0, sizeof(m_command.resurve));
	memset((void *)m_command.args.ui4, 0, sizeof(m_command.args.ui4));
}

int4 ShmCommandArea::sendRequest(const ShmCommand &cmd)
{
	if( m_state != Waiting ) {
		return ShmCmdErrCmdIsRunning;
	}

	memcpy((void*)&m_command, &cmd, MAX_LEN_OF_SHM_CMD);

	m_state = Request;

	return ShmCmdErrNoError;
}

int4 ShmCommandArea::recvResponse(ShmCommand &cmd)
{
	uint4 counter = 0;
	while( m_state != Acknowledge ) {
		counter++;
		SLEEP_USEC(10);
		if( counter >= 1000 ) {
			m_state = ErrorStop;
			return ShmCmdErrResponseTimeout;
		}
	}

	memcpy(&cmd, (const void*)&m_command, MAX_LEN_OF_SHM_CMD);

	m_state = Waiting;

	return ShmCmdErrNoError;
}

int4 ShmCommandArea::recvRequest(ShmCommand &cmd)
{
	if (m_state != Request) {
		return ShmCmdErrNotRequested;
	}

	memcpy(&cmd, (const void*)&m_command, MAX_LEN_OF_SHM_CMD);

	m_state = Running;

	return ShmCmdErrNoError;
}

int4 ShmCommandArea::sendResponse(const ShmCommand &cmd)
{
	if (m_state != Running) {
		return ShmCmdErrInvalidState;
	}

	memcpy((void*)&m_command, &cmd, MAX_LEN_OF_SHM_CMD);

	m_state = Acknowledge;

	return ShmCmdErrNoError;
}

