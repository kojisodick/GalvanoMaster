/*
 * PlMotionBlock.h
 *
 *  Created on: 2018/11/16
 *      Author: imao_yusuke
 */

#ifndef SRC_FPGA_PLMOTIONBLOCK_H_
#define SRC_FPGA_PLMOTIONBLOCK_H_

#include "Common.h"
#include "PlDef.h"

#define NUM_OF_PARAM_IN_PLBLOCK   (7)
#define SIZE_OF_PLBLOCK          (32)

enum PlMotionCode : uint4
{
	MtnCodeInvalid              = 0x00,
	MtnCodeScannerCmd           = 0x01,
	MtnCodeSystemSetting        = 0x02,
	MtnCodePulseWeightSetting   = 0x03,
	MtnCodeListControl          = 0x04,
	MtnCodeLinearMotion         = 0x05,
	MtnCodeArcMotion            = 0x06,
	MtnCodeWaitMotion           = 0x07,
	MtnCodeSpeedSetting         = 0x08,
	MtnCodeDelaySetting         = 0x09,
	MtnCodeLaserSetting         = 0x0A,
	MtnCodeLaserControl         = 0x0B,
	MtnCodeLaserStandbySetting  = 0x0C,
	MtnCodeLaserIoControl       = 0x0D,
	MtnCodeIoDout12Setting      = 0x0E,
	MtnCodeIoDout16Setting      = 0x0F,
	MtnCodeIoDout4Setting       = 0x10,
	MtnCodeIoDA1Setting         = 0x11,
	MtnCodeIoDA2Setting         = 0x12,
	MtnCodeRampDout12Setting    = 0x13,
	MtnCodeRampDA1Setting       = 0x14,
	MtnCodeRampDA2Setting       = 0x15,
	MtnCodeRampEndDout12Setting = 0x16,
	MtnCodeRampEndDA1Setting    = 0x17,
	MtnCodeRampEndDA2Setting    = 0x18,
	MtnCodeGatherControl        = 0x19,
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlScannerCtrlBlock
{
	PlMotionCode code;
	struct {
		uint4 data     : 20;
		uint4 id       : 8;
		uint4 targetCh : 4;
	} command;
	uint4  requestSts;
	uint4  unused[5];

	PlScannerCtrlBlock(): code(MtnCodeScannerCmd) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlSystemSettingBlock
{
	PlMotionCode code;
	fix8         focalLength;
	fix8         distanceMirrors;
	uint4        latticeInterval;
	uint4        geocompDisabled;
	uint4        unused[1];

	PlSystemSettingBlock(): code(MtnCodeSystemSetting) {}

};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlPulseWeightSettingBlock
{
	PlMotionCode code;
	fix8         pulseWeight[MAX_NUM_OF_AXIS];
	uint4        pulseWeightPerMilliOfZAxis;

	PlPulseWeightSettingBlock(): code(MtnCodePulseWeightSetting) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlListControlBlock
{
	PlMotionCode      code;
	PlListControlCode ctrlCode;
	uint4             unused[6];

	PlListControlBlock(): code(MtnCodeListControl) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlLinearMotionBlock
{
	PlMotionCode  code;
	PlMoveMode    mode;
	int4          cmdpos[MAX_NUM_OF_AXIS];  //[bit]
	fix8          arrivalCount;          //[control cycle]
	uint4         unused[1];

	PlLinearMotionBlock(): code(MtnCodeLinearMotion) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlArcMotionBlock
{
	PlMotionCode  code;
	PlMoveMode    mode;
	int4          cmdpos[2];   //[bit]
	int4          centerpos[2];//[bit]
	fix8          angle;

	PlArcMotionBlock(): code(MtnCodeArcMotion) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlWaitMotionBlock
{
	PlMotionCode code;
	uint4        count;
	uint4        unused[6];

	PlWaitMotionBlock(): code(MtnCodeWaitMotion) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlSpeedSettingBlock
{
	PlMotionCode code;
	fix8        jumpSpeed;
	fix8        markSpeed;
	fix8        gotoSpeed;
	uint4        unused[1];

	PlSpeedSettingBlock(): code(MtnCodeSpeedSetting) {}
};


/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlDelaySettingBlock
{
	PlMotionCode code;
	uint4        jump2mark;
	uint4        mark2jump;
	uint4        mark2mark;
	uint4        unused[4];

	PlDelaySettingBlock(): code(MtnCodeDelaySetting) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlLaserSettingBlock
{
	PlMotionCode code;
	uint4        mode;
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

	PlLaserSettingBlock(): code(MtnCodeLaserSetting) {}
};


/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlLaserStandbySettingBlock
{
	PlMotionCode code;
	uint4 halfPeriod;
	uint4 pulseWidth;
	uint4 unused[5];

	PlLaserStandbySettingBlock(): code(MtnCodeLaserStandbySetting) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlLaserControlBlock
{
	PlMotionCode code;
	uint4 active;
	uint4 output;
	uint4 count;
	uint4 unused[4];

	PlLaserControlBlock(): code(MtnCodeLaserControl) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlIoControlBlock
{
	PlMotionCode code;

	struct IoActive
	{
		uint4 dout12  : 1;
		uint4 dout16  : 1;
		uint4 dout4   : 1;
		uint4 da1     : 1;
		uint4 da2     : 1;
		uint4 unused : 27;
	} ioActive;

	struct RampActive
	{
		uint4 dout12  : 1;
		uint4 da1     : 1;
		uint4 da2     : 1;
		uint4 unused : 29;
	} rampActive[NumOfRampMode];

	uint4 unused[4];

	PlIoControlBlock(): code(MtnCodeLaserIoControl) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlIoSettingBlock
{
	PlMotionCode code;
	uint4 defaultVal;
	uint4 outputVal;
	uint4 unused[5];

	PlIoSettingBlock(LaserIoPort port): code(MtnCodeInvalid)
	{
		if     ( port == LaserIoDout12 ) code = MtnCodeIoDout12Setting;
		else if( port == LaserIoDout16 ) code = MtnCodeIoDout16Setting;
		else if( port == LaserIoDout4  ) code = MtnCodeIoDout4Setting;
		else if( port == LaserIoDa1    ) code = MtnCodeIoDA1Setting;
		else if( port == LaserIoDa2    ) code = MtnCodeIoDA2Setting;
	}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlRampSettingBlock
{
	PlMotionCode code;
	uint4 cycle;
	uint4 delta;
	uint4 targetVal;
	int4  tlen;
	uint4 unused[3];

	PlRampSettingBlock(RampMode mode, RampPort port): code(MtnCodeInvalid)
	{
		if( mode == RampStartMode ) {
			if     ( port == RampPortDout12 ) code = MtnCodeRampDout12Setting;
			else if( port == RampPortDa1    ) code = MtnCodeRampDA1Setting;
			else if( port == RampPortDa2    ) code = MtnCodeRampDA2Setting;
		}
		else if( mode == RampEndMode ) {
			if     ( port == RampPortDout12 ) code = MtnCodeRampEndDout12Setting;
			else if( port == RampPortDa1    ) code = MtnCodeRampEndDA1Setting;
			else if( port == RampPortDa2    ) code = MtnCodeRampEndDA2Setting;
		}
	}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlGatherCtrlBlock
{
	PlMotionCode code;
	uint4 trigger;
	uint4 unused[6];

	PlGatherCtrlBlock(): code(MtnCodeGatherControl) {}
};

/*-------------------------------------------------------------
 *
 *-------------------------------------------------------------*/
struct PlMotionBlock
{
	PlMotionCode code;
	uint4 param[NUM_OF_PARAM_IN_PLBLOCK];

	PlMotionBlock():code(MtnCodeInvalid), param()
	{
	}

	template <class T>
	void setData(const T &obj)
	{
		uint4 *ptr = (uint4*)&obj;
		code = (PlMotionCode)ptr[0];
		memcpy(param, &ptr[1], sizeof(param));
	}

	template <class T>
	PlMotionBlock(const T &obj)
	{
		this->setData(obj);
	}

	template <class T>
	PlMotionBlock &operator= (const T &obj)
	{
		this->setData(obj);
		return (*this);
	}
};




#endif /* SRC_FPGA_PLMOTIONBLOCK_H_ */
