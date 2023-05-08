/*
 * ShmCommand.h
 *
 *  Created on: 2018/12/25
 *      Author: imao_yusuke
 */

#ifndef SRC_SHAREDMEMORY_SHMCOMMAND_H_
#define SRC_SHAREDMEMORY_SHMCOMMAND_H_

#include "Common.h"

enum ShmCommandCode: uint4
{
	ShmCmdCodeInvalid = 0,
	ShmCmdCodeSetGatSignal,
	ShmCmdCodeSetGatConfig,
	ShmCmdCodeStartGat,
	ShmCmdCodeEndGat,
	ShmCmdCodeUploadGatData,
	ShmCmdCodeWaitForStartOfGat,
	ShmCmdCodeWaitForEndOfGat,
	ShmCmdCodeGetGatStatus,
	ShmCmdCodeGetGatSignal,
	ShmCmdCodeGetGatConfig,

	MaxNumOfShmCmd //NOTE: When adding code, please add it before this
};

enum ShmCommandError: uint4
{
	ShmCmdErrNoError = 0,
	ShmCmdErrInvalidCmdCode,
	ShmCmdErrInvalidCmdData,
	ShmCmdErrCmdIsRunning,
	ShmCmdErrInvalidState,
	ShmCmdErrNotRequested,
	ShmCmdErrResponseTimeout,
};


#define LEN_OF_SHM_CMD_HEADER    (16)
#define LEN_OF_SHM_CMD_BODY      (48)
#define MAX_LEN_OF_SHM_CMD       (LEN_OF_SHM_CMD_HEADER+LEN_OF_SHM_CMD_BODY)

struct ShmCommand
{
	// Header [16 byte]
	ShmCommandCode  code;
	ShmCommandError error;
	uint4 resurve[2];

	// Body   [48 byte]
	union Arguments
	{
		uint4 ui4[LEN_OF_SHM_CMD_BODY/4];
		real4 rl4[LEN_OF_SHM_CMD_BODY/4];
		real8 rl8[LEN_OF_SHM_CMD_BODY/8];
	} args;


	ShmCommand()
	: code(ShmCmdCodeInvalid),
	  error(ShmCmdErrNoError),
	  resurve()
	{
		memset((void *)args.ui4, 0, sizeof(args.ui4));
	}
};

typedef ShmCommand::Arguments SCmdArgs;


#endif /* SRC_SHAREDMEMORY_SHMCOMMAND_H_ */
