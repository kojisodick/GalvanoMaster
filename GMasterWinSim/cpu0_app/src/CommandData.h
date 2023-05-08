/*
 * CommandData.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMAND_COMMANDDATA_H_
#define SRC_COMMAND_COMMANDDATA_H_


#include <Logger/Logger.h>
#include "Common.h"

extern Logger *gp_log;

#define MAX_NUM_OF_GALVANO_CMD    (256)

#define MIN_VAL_OF_CMDCODE        (0)
#define MAX_VAL_OF_CMDCODE        (0xFF)

#define CTRLCMD_BASECODE          (0x01)
#define LISTCMD_BASECODE          (0x80)


enum CommandCode: uint1
{
	CmdCodeInvalid                 = 0x00,

	/* Control Commands */
	CmdCodeCtrlSelect              = 0x01,
	CmdCodeCtrlLoadPrgFile         = 0x02,
	CmdCodeCtrlLoadCorrection      = 0x03,
	CmdCodeCtrlCountCards          = 0x04,
	CmdCodeCtrlGetDllVersion       = 0x05,
	CmdCodeCtrlGetHexVersion       = 0x06,
	CmdCodeCtrlGetVersion          = 0x07,
	CmdCodeCtrlSetLaserMode        = 0x08,
	CmdCodeCtrlSetLaserParam       = 0x09,
	CmdCodeCtrlDisableLaser        = 0x0A,
	CmdCodeCtrlEnableLaser         = 0x0B,
	CmdCodeCtrlSetStandby          = 0x0C,
	CmdCodeCtrlLaserOn             = 0x0D,
	CmdCodeCtrlLaserOff            = 0x0E,
	CmdCodeCtrlSetLaserDelay       = 0x0F,
	CmdCodeCtrlSetScannerDelay     = 0x10,
	CmdCodeCtrlSetJumpSpeed        = 0x11,
	CmdCodeCtrlSetMarkSpeed        = 0x12,
	CmdCodeCtrlGotoXY              = 0x13,
	CmdCodeCtrlGetXYPos            = 0x14,
	CmdCodeCtrlGotoXYZ             = 0x15,
	CmdCodeCtrlGetXYZPos           = 0x16,
	CmdCodeCtrlSetPortEnable       = 0x17,
	CmdCodeCtrlSetPortDefault      = 0x18,
    CmdCodeCtrlReadIoPort          = 0x19,
	CmdCodeCtrlWriteIoPort         = 0x1A,
	CmdCodeCtrlRead4BitPort        = 0x1B,
	CmdCodeCtrlWrite4BitPort       = 0x1C,
	CmdCodeCtrlWrite12BitPort      = 0x1D,
	CmdCodeCtrlWriteDa1            = 0x1E,
	CmdCodeCtrlWriteDa2            = 0x1F,
	CmdCodeCtrlRampMode            = 0x20,
	CmdCodeCtrlRampParam           = 0x21,
	CmdCodeCtrlRampEndMode         = 0x22,
	CmdCodeCtrlRampEndParam        = 0x23,
	CmdCodeCtrlSetCtrlMode         = 0x24,
	CmdCodeCtrlSelectList          = 0x25,
	CmdCodeCtrlGetStatStopInfo     = 0x26,
	CmdCodeCtrlSetExtStartDelay    = 0x27,
	CmdCodeCtrlGetDelayCount       = 0x28,
	CmdCodeCtrlSetStartList        = 0x29,
	CmdCodeCtrlExecuteList         = 0x2A,
	CmdCodeCtrlExecuteAtPoint      = 0x2B,
	CmdCodeCtrlStopExecution       = 0x2C,
	CmdCodeCtrlStopList            = 0x2D,
	CmdCodeCtrlRestartList         = 0x2E,
	CmdCodeCtrlGetStatus           = 0x2F,
	CmdCodeCtrlReadStatus          = 0x30,
	CmdCodeCtrlGetWaitStatus       = 0x31,
	CmdCodeCtrlReleaseWait         = 0x32,
	CmdCodeCtrlAutoChange          = 0x33,
	CmdCodeCtrlStartLoop           = 0x34,
	CmdCodeCtrlQuitLoop            = 0x35,
	CmdCodeCtrlSetInputPtr         = 0x36,
	CmdCodeCtrlGetInputntr         = 0x37,
	CmdCodeCtrlSimEncoder          = 0x38,
	CmdCodeCtrlGetMarkInfo         = 0x39,
	CmdCodeCtrlGetEncoder          = 0x3A,
	CmdCodeCtrlXY2Delay            = 0x3B,
	CmdCodeCtrlGetValue            = 0x3C,
	CmdCodeCtrlSetGatSignal        = 0x3D,
	CmdCodeCtrlSetGatConfig        = 0x3E,
	CmdCodeCtrlSetGatTrigger       = 0x3F,
	CmdCodeCtrlGetGatStatus        = 0x40,
	CmdCodeCtrlGetGatData          = 0x41,
	CmdCodeCtrlGetGatDataEx        = 0x42,
	CmdCodeCtrlGetTime             = 0x43,
	CmdCodeCtrlGetBitWeight        = 0x44,
	CmdCodeCtrlFocusShift          = 0x45,
	CmdCodeCtrlSetVscParam         = 0x46,
	CmdCodeCtrlSetVpcParam         = 0x47,

	/* add sodick */
    CmdCodeCtrlDwloadCorrection    = 0x50,
    CmdCodeCtrlGetErrInfo          = 0x51,
    CmdCodeCtrlGetSysTime          = 0x52,
    CmdCodeCtrlSetGotoSpeed        = 0x53,
    CmdCodeCtrlGetGatSignal        = 0x54,
    CmdCodeCtrlGetGatConfig        = 0x55,
    CmdCodeCtrlGetVscParam         = 0x56,
    CmdCodeCtrlGetMasterParam      = 0x57,
    CmdCodeCtrlGetValues           = 0x58,
    CmdCodeCtrlGetFpgaReg          = 0x59,
	CmdCodeCtrlGetFpgaBlock        = 0x5A,
	CmdCodeCtrlGetFpgaInfo         = 0x5B,
	CmdCodeCtrlGetListInfo         = 0x5C,
	CmdCodeCtrlSetOpticalInfo      = 0x5D,
	CmdCodeCtrlGetOpticalInfo      = 0x5E,
	CmdCodeCtrlSetBitWeight        = 0x5F,
	CmdCodeCtrlChangeStatus        = 0x60,
	CmdCodeCtrlEnableGather        = 0x61,
	CmdCodeCtrlDisableGather       = 0x62,
	CmdCodeCtrlSetVscMode          = 0x63,
	CmdCodeCtrlSetVscLimit         = 0x64,

	CmdCodeCtrlEmgStop             = 0x70,
	CmdCodeCtrlServoOn             = 0x71,
	CmdCodeCtrlServoOff            = 0x72,
    CmdCodeCtrlSetSimMode          = 0x73,
    CmdCodeCtrlClearErr            = 0x74,

	/* List Commands */
	CmdCodeListJumpAbs             = 0x80,
	CmdCodeListJumpRel             = 0x81,
	CmdCodeListMarkAbs             = 0x82,
	CmdCodeListMarkRel             = 0x83,
	CmdCodeListTimedJumpAbs        = 0x84,
	CmdCodeListTimedJumpRel        = 0x85,
	CmdCodeListTimedMarkAbs        = 0x86,
	CmdCodeListTimedMarkRel        = 0x87,
	CmdCodeListJumpAbs3D           = 0x88,
	CmdCodeListJumpRel3D           = 0x89,
	CmdCodeListMarkAbs3D           = 0x8A,
	CmdCodeListMarkRel3D           = 0x8B,
	CmdCodeListTimedJumpAbs3D      = 0x8C,
	CmdCodeListTimedJumpRel3D      = 0x8D,
	CmdCodeListTimedMarkAbs3D      = 0x8E,
	CmdCodeListTimedMarkRel3D      = 0x8F,
	CmdCodeListArcAbs              = 0x90,
	CmdCodeListArcRel              = 0x91,
	CmdCodeListSetStandby          = 0x92,
	CmdCodeListSetLaserTiming      = 0x93,
	CmdCodeListLaserOn             = 0x94,
	CmdCodeListLaserSignalOn       = 0x95,
	CmdCodeListLaserSignalOff      = 0x96,
	CmdCodeListSetLaserDelay       = 0x97,
	CmdCodeListSetScannerDelay     = 0x98,
	CmdCodeListSetJumpSpeed        = 0x99,
	CmdCodeListSetMarkSpeed        = 0x9A,
	CmdCodeListSetCircular         = 0x9B,
	CmdCodeListWriteIoPort         = 0x9C,
	CmdCodeListWrite4BitPort       = 0x9D,
	CmdCodeListWrite12BitPort      = 0x9E,
	CmdCodeListWriteDa1            = 0x9F,
	CmdCodeListWriteDa2            = 0xA0,
	CmdCodeListRampMode            = 0xA1,
	CmdCodeListRampParam           = 0xA2,
	CmdCodeListRampEndMode         = 0xA3,
	CmdCodeListRampEndParam        = 0xA4,
	CmdCodeListSetCtrlMode         = 0xA5,
	CmdCodeListSetEndOfList        = 0xA6,
	CmdCodeListSetWait             = 0xA7,
	CmdCodeListListNop             = 0xA8,
	CmdCodeListLongDelay           = 0xA9,
	CmdCodeListSetFlyX             = 0xAA,
	CmdCodeListSetFlyY             = 0xAB,
	CmdCodeListSetFlyXY            = 0xAC,
	CmdCodeListFlyReturn           = 0xAD,
	CmdCodeListSetGatTrigger       = 0xAE,
	CmdCodeListRestartTimer        = 0xAF,
	CmdCodeListFocusShift          = 0xB0,
	CmdCodeListSetVscParam         = 0xB1,
	CmdCodeListSetVpcParam         = 0xB2,
	CmdCodeListSetVscMode          = 0xB3,
	CmdCodeListSetVscLimit         = 0xB4,

};




#define SIZE_OF_CMDDATA_HEADER    (8)
#define SIZE_OF_CMDDATA_ARGS      (32)
#define SIZE_OF_CMDDATA           (SIZE_OF_CMDDATA_HEADER+SIZE_OF_CMDDATA_ARGS)

struct CommandData
{
	// Header
	uint1 code;
	uint1 result;
	uint1 counter;
    uint1 unused[5];

	union Arguments
	{
		uint1 ui1[SIZE_OF_CMDDATA_ARGS];
		uint4 ui4[SIZE_OF_CMDDATA_ARGS/4];
		int4  si4[SIZE_OF_CMDDATA_ARGS/4];
		real4 rl4[SIZE_OF_CMDDATA_ARGS/4];
		real8 rl8[SIZE_OF_CMDDATA_ARGS/8];
	} args;


	CommandData(){ this->clear(); }
	~CommandData(){}

	void clear()
	{
		code      = CmdCodeInvalid;
        result    = 0;
        counter   = 0;
        memset( unused, 0, sizeof(unused));
        memset( &args,  0, sizeof(args)  );
	}

	void showHeader()
	{
		gp_log->debug("Command: code=0x%x result=%d counter=%d", code, result, counter);
	}
};

typedef CommandData::Arguments CmdArgs;


#endif /* SRC_COMMAND_COMMANDDATA_H_ */
