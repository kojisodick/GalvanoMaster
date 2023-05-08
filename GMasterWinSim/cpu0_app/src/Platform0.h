/*
 * Platform.h
 *
 *  Created on: 2018/11/30
 *      Author: imao_yusuke
 */

#ifndef SRC_SYSTEM_PLATFORM_H_
#define SRC_SYSTEM_PLATFORM_H_

#include "Common.h"

class Platform0
{
public:
	enum ErrorCode {
		NoError = 0,
		SCUTimerSetupFailed,
		SysErrPlatformInitFailed
	};

public:
	Platform0() {};
	~Platform0() {};

	static Platform0 * obj();

	virtual int4 init() = 0;
	virtual void cleanup() = 0;
	virtual void enableInterrupt() = 0;
	virtual void disableInterrupt() = 0;
};



#endif /* SRC_SYSTEM_PLATFORM_H_ */
