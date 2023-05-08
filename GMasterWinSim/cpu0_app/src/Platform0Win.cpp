/*
* PlatformWin.cpp
*
*  Created on: 2018/12/04
*      Author: imao_yusuke
*/

#ifndef __arm__

#include "Platform0Win.h"
#include "MotionManager.h"
#include "ServerWin.h"

#define _WINSOCKAPI_
#include <Windows.h>
#include <process.h>

unsigned int Platform0Win::DummyThread0(void *param)
{
	while (1) {
		ServerWin::obj()->runTcpTask();
	}
    return 0;
}


Platform0Win::Platform0Win()
	: Platform0()
{
}

Platform0Win::~Platform0Win()
{
	CloseHandle(m_hThread);

	HANDLE hMutex = MotionManager::obj()->mutex();
    CloseHandle(hMutex);
}

int4 Platform0Win::init()
{    
	SetLastError(ERROR_SUCCESS);

    m_hThread = (HANDLE)_beginthreadex(
        NULL,
        0,
		Platform0Win::DummyThread0,
        NULL,
        0,
        NULL);

    if( m_hThread == NULL ) {
        printf("[Error] _beginthreadex failed. (%d)\n", GetLastError());
        return SysErrPlatformInitFailed;
    }

    return NoError;
}

void  Platform0Win::cleanup()
{
}

void Platform0Win::enableInterrupt()
{
}


void Platform0Win::disableInterrupt()
{
}


#endif
