/*
 * Common.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMON_COMMON_H_
#define SRC_COMMON_COMMON_H_


#include "TypeDef.h"
#include "SystemDef.h"
#include "smath.h"
#include "ErrorCode.h"

#ifdef __arm__
#include "sleep.h"
#define SLEEP_USEC(usec) usleep(usec)
#define FPRINTF f_printf
#define PRINTF printf
#else
#include <Windows.h>
#define SLEEP_USEC(usec) Sleep(usec)  // use MSEC instead of USEC on Windows environment
#define FPRINTF fprintf
#define PRINTF printf
#endif

#include <stdio.h>



#endif /* SRC_COMMON_COMMON_H_ */
