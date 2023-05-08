/*
* PlatformWin.cpp
*
*  Created on: 2018/12/04
*      Author: imao_yusuke
*/

#ifndef __arm__

#include "Platform1Win.h"
#include "MotionManager.h"
#include <process.h>

unsigned int __stdcall DummyThread1(void *param)
{
	return 0;
}


Platform1Win::Platform1Win()
	: Platform1()
{
}

Platform1Win::~Platform1Win()
{
}

int4 Platform1Win::init(uint4 controlCycle)
{
	// nothing to do for Windows
    return NoError;
}

void  Platform1Win::cleanup()
{
}




#endif
