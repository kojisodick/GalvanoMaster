/*
 * Platform.h
 *
 *  Created on: 2018/11/29
 *      Author: imao_yusuke
 */

#ifndef SRC_PLATFORM1_H_
#define SRC_PLATFORM1_H_

#include "Common.h"

class Platform1
{
public:
	enum ErrorCode
	{
		NoError = 0,
		SCUTimerSetupFailed,
		SysErrPlatformInitFailed
	};

	Platform1() {}
	~Platform1(){}

	static Platform1 * obj();

	virtual int4 init(uint4 controlCycle) = 0;
	virtual void cleanup() = 0;

private:

};

#endif /* SRC_PLATFORM1_H_ */
