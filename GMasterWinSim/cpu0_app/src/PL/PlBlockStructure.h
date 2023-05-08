/*
 * PlBlockStructure.h
 *
 *  Created on: 2020/03/13
 *      Author: imao_yusuke
 */

#ifndef SRC_PL_PLBLOCKSTRUCTURE_H_
#define SRC_PL_PLBLOCKSTRUCTURE_H_

#include "Common.h"

/* -------------------------------------------------
 * PL Motion Block
 * size: 32byte
 */
struct PlBlock
{
	enum Code: uint4
	{
		CodeInvalid              = 0x00,
		CodeDriverControl        = 0x01,
		CodeOpticalParamConf     = 0x02,
		CodeScaleFactorConf      = 0x03,
		CodeListControl          = 0x04,
		CodeLinearMove           = 0x05, // 
		CodeArcMove              = 0x06, //
		CodeWait                 = 0x07,
		CodeSpeedConf            = 0x08,
		CodeDelayConf            = 0x09,
		CodeActLaserConf         = 0x0A,
		CodeLaserControl         = 0x0B,
		CodeSubLaserConf         = 0x0C,
		CodeLaserIoControl       = 0x0D,
		CodeLaserDout12Conf      = 0x0E,
		CodeLaserDout16Conf      = 0x0F,
		CodeLaserDout4Conf       = 0x10,
		CodeLaserDA1Conf         = 0x11,
		CodeLaserDA2Conf         = 0x12,
		CodeRampDout12Conf       = 0x13,
		CodeRampDA1Conf          = 0x14,
		CodeRampDA2Conf          = 0x15,
		CodeRampEndDout12Conf    = 0x16,
		CodeRampEndDA1Conf       = 0x17,
		CodeRampEndDA2Conf       = 0x18,
		CodeGatherControl        = 0x19,
	} code;

	uint4 param[7];

	PlBlock(): code(CodeInvalid)
	{
		memset(param, 0 , sizeof(param));
	}

	void init(Code code=CodeInvalid)
	{
		this->code = code;
		memset(param, 0 , sizeof(param));
	}
};

/* -------------------------------------------------
 * PL Driver Control Block
 * size: 32byte
 */
struct PlBlockDriverControl
{
	PlBlockDriverControl()
	{
		code = PlBlock::CodeDriverControl;
		command.data     = 0;
		command.id       = 0;
		command.targetCh = 0;
		stausID          = 0;
		memset(unused, 0, sizeof(unused));
	}


	enum CommandData
	{
		CmdDataOff = 0x00000,
		CmdDataOn  = 0x00001,
	};

	enum CommandID
	{
		CmdIdDummy           = 0x00,
		CmdIdEmergencyStop   = 0x01,
		CmdIdCalibration     = 0x02,
		CmdIdClearAlarm      = 0x03,
		CmdIdChangeXY2Status = 0x10,
	};

	enum CommandTargetChannel
	{
		CmdTargetCh1 = 0x01,
		CmdTargetCh2 = 0x02,
		CmdTargetCh3 = 0x04,
		CmdTargetCh4 = 0x08,
	};

	PlBlock::Code code;
	struct Command
	{
		uint4 data     : 20;
		uint4 id       : 8;
		uint4 targetCh : 4;
	} command;
	uint4  stausID;
	uint4  unused[5];
};

/* PL Optical System Configuration Block
 * size: 32byte
 */
struct PlBlockOpticalSystemConf
{
	PlBlockOpticalSystemConf()
	{
		code   = PlBlock::CodeOpticalParamConf;
		param1 = 0.0;
		param2 = 0.0;
		latticeInterval = 0;
		disabledGeoComp = 0;
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	fix8         param1;
	fix8         param2;
	uint4        latticeInterval;
	uint4        disabledGeoComp;
	uint4        unused[1];
};

/* PL Scale-factor Configuration Block
 * size: 32byte
 */
struct PlBlockScaleFactorConf
{
	PlBlockScaleFactorConf()
	{
		code = PlBlock::CodeScaleFactorConf;
		memset(value,  0, sizeof(value));
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	fix8          value[MAX_NUM_OF_AXIS];
	uint4         unused[1];
};

/*
 *
 */
struct PlBlockListControl
{
	PlBlockListControl()
	{
		code = PlBlock::CodeListControl;
		cmd  = CmdDummy;
		memset(unused, 0, sizeof(unused));
	}

	enum Command
	{
		CmdPauseList  = 0x00,
		CmdBeginList  = 0x01,
		CmdEndList    = 0x02,
		CmdDummy      = 0xFF,
	};

	PlBlock::Code code;
	Command       cmd;
	uint4         unused[6];

};

/*
 *
 */
struct PlBlockLinearMove
{
	PlBlockLinearMove()
	{
		code = PlBlock::CodeLinearMove;
		mode = ModeJump;
		arrivalCount = 0.0;
		memset(cmdPos, 0, sizeof(cmdPos));
		memset(unused, 0, sizeof(unused));
	}

	enum Mode
	{
		ModeSkyWriting = 0x00,
		ModeMark       = 0x01,
		ModeJump       = 0x10,
	};

	PlBlock::Code code;
	Mode          mode;
	int4          cmdPos[MAX_NUM_OF_AXIS];  //[bit]
	fix8          arrivalCount;          //[control cycle]
	uint4         unused[1];
};

/*
 *
 */
struct PlBlockArcMove
{
	PlBlockArcMove()
	{
		code  = PlBlock::CodeArcMove;
		mode  = ModeMark;
		angle = 0.0;
		memset(cmdPos, 0, sizeof(cmdPos));
		memset(centerpos, 0, sizeof(centerpos));

	}

	enum Mode
	{
		ModeSkyWriting = 0x00,
		ModeMark       = 0x01,
	};

	PlBlock::Code code;
	Mode          mode;
	int4          cmdPos[2];   //[bit]
	int4          centerpos[2];//[bit]
	fix8          angle;
};

/*
 *
 */
struct PlBlockWait
{
	PlBlockWait()
	{
		code  = PlBlock::CodeWait;
		count = 0;
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	uint4         count;
	uint4         unused[6];
};

/*
 *
 */
struct PlBlockSpeedConf
{
	PlBlockSpeedConf()
	{
		code = PlBlock::CodeSpeedConf;
		jumpSpeed = 1.0;
		markSpeed = 1.0;
		gotoSpeed = 1.0;
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	fix8          jumpSpeed;
	fix8          markSpeed;
	fix8          gotoSpeed;
	uint4         unused[1];
};

/*
 *
 */
struct PlBlockDelayConf
{
	PlBlockDelayConf()
	{
		code = PlBlock::CodeDelayConf;
		jump2mark = 0;
		mark2jump = 0;
		mark2mark = 0;

		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	uint4         jump2mark;
	uint4         mark2jump;
	uint4         mark2mark;
	uint4         unused[4];
};

/*
 *
 */
struct PlBlockLaserConf
{
	PlBlockLaserConf()
	{
		code         = PlBlock::CodeActLaserConf;
		mode         = ModeCO2;
		logic.gate   = 0;
		logic.pulse1 = 0;
		logic.pulse2 = 0;
		logic.unused = 0;
		halfPeriod   = 0xffffff;
		pulse1Width  = 2;
		pulse2Width  = 3;
		onDelay      = 0;
		offDelay     = 0;
	}


	enum Mode
	{
		ModeCO2  = 0,
		ModeYAG1 = 1,
		ModeYAG2 = 2,
		ModeYAG3 = 3,
	};

	enum Logic
	{
		LogicActHigh = 0,
		LogicActLow  = 1,
	};


	PlBlock::Code code;
	uint4         mode;
	struct
	{
		uint4 gate    : 1;
		uint4 pulse1  : 1;
		uint4 pulse2  : 1;
		uint4 unused  : 29;
	} logic;
	uint4        halfPeriod;
	uint4        pulse1Width;
	uint4        pulse2Width;
	uint4        onDelay;
	uint4        offDelay;
};

struct PlBlockLaserControl
{
	PlBlockLaserControl()
	{
		code     = PlBlock::CodeLaserControl;
		active   = 0;
		signalOn = 0;
		count    = 0;

		memset(unused, 0, sizeof(unused));
	}


	PlBlock::Code code;
	uint4         active;
	uint4         signalOn;
	uint4         count;
	uint4         unused[4];

};

struct PlBlockLaserStandbyConf
{
	PlBlockLaserStandbyConf()
	{
		code = PlBlock::CodeSubLaserConf;
		halfPeriod = 0xffffff;
		pulseWidth = 2;
		memset(unused, 0, sizeof(unused));
	}


	PlBlock::Code code;
	uint4         halfPeriod;
	uint4         pulseWidth;
	uint4         unused[5];

};

struct PlBlockLaserIOControl
{
	PlBlockLaserIOControl()
	{
		code = PlBlock::CodeLaserIoControl;
		io.dout12 = 0;
		io.dout16 = 0;
		io.dout4  = 0;
		io.da1    = 0;
		io.da2    = 0;
		io.unused = 0;

		ramp.dout12 = 0;
		ramp.da1    = 0;
		ramp.da2 = 0;
		ramp.unused = 0;

		rampEnd.dout12 = 0;
		rampEnd.da1    = 0;
		rampEnd.da2    = 0;
		rampEnd.unused = 0;

		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	struct IoActive
	{
		uint4 dout12  : 1;
		uint4 dout16  : 1;
		uint4 dout4   : 1;
		uint4 da1     : 1;
		uint4 da2     : 1;
		uint4 unused : 27;
	} io;

	struct RampActive
	{
		uint4 dout12  : 1;
		uint4 da1     : 1;
		uint4 da2     : 1;
		uint4 unused : 29;
	} ramp, rampEnd;

	uint4 unused[4];
};

struct PlBlockLaserIOParam
{
	PlBlockLaserIOParam()
	{
		code       = PlBlock::CodeLaserDout12Conf;
		defaultVal = 0;
		outputVal  = 0;
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	uint4         defaultVal;
	uint4         outputVal;
	uint4         unused[5];
};

struct PlBlockRampParam
{
	PlBlockRampParam()
	{
		code   = PlBlock::CodeRampDout12Conf;
		cycle  = 0;
		delta  = 0;
		endVal = 0;
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	uint4         cycle;
	uint4         delta;
	uint4         endVal;
	uint4         unused[4];
};

struct PlBlockRampEndParam
{
	PlBlockRampEndParam()
	{
		code = PlBlock::CodeRampEndDout12Conf;
		cycle  = 0;
		delta  = 0;
		endVal = 0;
		tlen   = 0;
		memset(unused, 0, sizeof(unused));
	}

	PlBlock::Code code;
	uint4         cycle;
	uint4         delta;
	uint4         endVal;
	int4          tlen;
	uint4         unused[3];
};

struct PlBlockGatherControl
{
	PlBlockGatherControl()
	{
		code = PlBlock::CodeGatherControl;
		trigger = 0;
		memset(unused, 0, sizeof(unused));
	}

	enum
	{
		StopGather  = 0,
		StartGather = 1,
	};

	PlBlock::Code code;
	uint4         trigger;
	uint4         unused[6];
};

#endif /* SRC_PL_PLBLOCKSTRUCTURE_H_ */
