/*
 * ErrorCode.h
 *
 *  Created on: 2020/08/13
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMON_ERRORCODE_H_
#define SRC_COMMON_ERRORCODE_H_

enum ErrorCode: int4
{
	NoError                      = 0x00,
	CommandInvalidCode           = 0x01,
	CommandInvalidArgument       = 0x02,
	LANConnectionFailed          = 0x03,
	LANCommunicaitonError        = 0x04,
	LANNotConnected              = 0x05,
	CorrectionFileNotFound       = 0x06,
	CorrectionFileInvalidFormat  = 0x07,
	ListIsNotLoading             = 0x08,
	ListIsNotReady               = 0x09,
	ListIsNotRunning             = 0x0a,
	ListOverflowed               = 0x0b,
	ListIsRunning                = 0x0c,
	ListInvalidWriteIndex        = 0x0d,
	ListInvalidReadIndex         = 0x0e,
	PLInitFailed                 = 0x0f,
	PLIsBusy                     = 0x10,
	PLInvalidStatus              = 0x11,
	PLError                      = 0x12,
	DriverError                  = 0x13,
};



#endif /* SRC_COMMON_ERRORCODE_H_ */
