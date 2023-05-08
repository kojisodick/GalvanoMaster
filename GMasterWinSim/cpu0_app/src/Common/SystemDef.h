/*
 * SystemDef.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMON_SYSTEMDEF_H_
#define SRC_COMMON_SYSTEMDEF_H_

//#define DEBUG_PS_BLOCK
//#define DEBUG_ACT_STS
//#define DEBUG_GAL96

struct Version
{
	unsigned char major;
	unsigned char minor;
	unsigned char revision;
	unsigned char build;
};

struct Date
{
	unsigned char year;
	unsigned char month;
	unsigned char day;
};

const static Version  BIN_VERSION  = { 1, 0, 4, 0 };
const static Version  PS_VERSION   = { 1, 0, 4, 0 };
const static Date     RELEASE_DATE = { 22, 5, 10 };

const static char     VERSION_COMMENT[] = "Fixed clr_focus_shift/lst_focus_shift commands.";

enum Axis : unsigned long
{
	X = 0,
	Y,
	Z,

	MaxNumOfAxes
};

#define MAX_NUM_OF_AXIS  (3)

enum WeightUnit : unsigned long
{
	BitPerMilli = 0,
	MilliPerBit,
	PulsePerRadian,
	PulsePerBit,
};


enum Coordinate: unsigned long
{
	Absolute = 0,
	Relative = 1
};

enum Dimension: unsigned long
{
	TwoD   = 2,
	ThreeD = 3
};

enum LaserMode: unsigned long
{
	LaserModeCO2  = 0,
	LaserModeYAG1,
	LaserModeYAG2,
	LaserModeYAG3,

	NumOfLaserMode
};

enum LaserSignal: unsigned long
{
	LaserSignalGate   = 0,
	LaserSignalPulse1,
	LaserSignalPulse2,

	NumOfLaserSignal
};

enum SignalLogic: unsigned long
{
	ActiveHigh = 0,
	ActiveLow  = 1,
};

enum LaserIoPort: unsigned long
{
	LaserIoDout12 = 0,
	LaserIoDout16,
	LaserIoDout4,
	LaserIoDa1,
	LaserIoDa2,

	NumOfLaserIoPort
};

enum RampMode: unsigned long
{
	RampStartMode = 0,
	RampEndMode,

	NumOfRampMode
};

enum RampPort: unsigned long
{
	RampPortDout12 = 0,
	RampPortDa1,
	RampPortDa2,

	NumOfRampPort
};

enum ListBoxCh : unsigned long
{
	ListBox1 = 0,
	ListBox2,

	NumOfListBox
};

enum AppNumber: unsigned long
{
	Cpu0App = 0,
	Cpu1App = 1,

	NumOfApp,
};

enum AppState: unsigned long
{
	AppWaiting = 0,
	AppRunning = 1,
	AppErrStop = 2,
};


typedef enum GatheringItemNumber : unsigned long
{
	GatItemBoardStatus1 = 0,
	GatItemBoardStatus2,

	//DPRAM
	GatItemPsCommand,
	GatItemWriteIndex,

	GatItemPlVersion,
	GatItemPhaseCounter,
	GatItemGatheringFlag,

	GatItemPlStatus,
	GatItemReadIndex,
	GatItemMoveIndex,
	GatItemHaltNumber,
	GatItemCmdPosBit_X,
	GatItemCmdPosBit_Y,
	GatItemCmdPosBit_Z,
	GatItemCmdPosPulse_X,
	GatItemCmdPosPulse_Y,
	GatItemCmdPosPulse_Z,
	GatItemActPosPulse_X,
	GatItemActPosPulse_Y,
	GatItemActPosPulse_Z,
	GatItemFirstMotorErr_X,
	GatItemFirstMotorErr_Y,
	GatItemFirstMotorErr_Z,
	GatItemFirstPlErr,
	GatItemLatestMotorErr_X,
	GatItemLatestMotorErr_Y,
	GatItemLatestMotorErr_Z,
	GatItemLatestPlErr,
	GatItemActData1,
	GatItemActData2,
	GatItemActData3,
	GatItemActData4,
	GatItemActStatus0,
	GatItemActStatus1,
	GatItemActStatus2,
	GatItemActStatus3,
	GatItemActStatus4,
	GatItemBlockLength,
	GatItemBlockCount,
	GatItemCableDelay,
	GatItemActualLaserDout12Val,
	GatItemActualLaserDio16Val,
	GatItemActualLaserDio4AVal,
	GatItemActualLaserDa1Da2Val,

	GatItemBlockMode,
	GatItemPulseWeight_X,
	GatItemPulseWeight_Y,
	GatItemPulseWeight_Z,
	GatItemFocalLength,
	GatItemMirrorDistance,
	GatItemBlockMovemode,
	GatItemBlockCmdPos_X,
	GatItemBlockCmdPos_Y,
	GatItemBlockCmdPos_Z,
	GatItemBlockArrivalCnt,
	GatItemBlockCenterPos_X,
	GatItemBlockCenterPos_Y,
	GatItemBlockArcAngle,
	GatItemBlockWaitCounts,
	GatItemJumpSpeed,
	GatItemMarkSpeed,
	GatItemGotoSpeed,
	GatItemJump2MarkDelay,
	GatItemMark2MarkDelay,
	GatItemMark2JumpDelay,
	GatItemLaserLogic,
	GatItemLaserHalfPeriod,
	GatItemLaserPulse1Width,
	GatItemLaserPulse2Width,
	GatItemLaserOnDelay,
	GatItemLaserOffDelay,
	GatItemLaserTime,
	GatItemStandbyLaserHalfPeriod,
	GatItemStandbyLaserPulseWidth,
	GatItemIoEnable,
	GatItemIoDout12SettingVal,
	GatItemIoDout16SettingVal,
	GatItemIoDout4SettingVal,
	GatItemIoDa1SettingVal,
	GatItemIoDa2SettingVal,

	GatItemRampStartDout12Cycle,
	GatItemRampStartDout12Delta,
	GatItemRampStartDout12EndVal,
	GatItemRampStartDa1Cycle,
	GatItemRampStartDa1Delta,
	GatItemRampStartDa1EndVal,
	GatItemRampStartDa2Cycle,
	GatItemRampStartDa2Delta,
	GatItemRampStartDa2EndVal,

	GatItemRampEndDout12Cycle,
	GatItemRampEndDout12Delta,
	GatItemRampEndDout12EndVal,
	GatItemRampEndDout12TLen,
	GatItemRampEndDa1Cycle,
	GatItemRampEndDa1Delta,
	GatItemRampEndDa1EndVal,
	GatItemRampEndDa1TLen,
	GatItemRampEndDa2Cycle,
	GatItemRampEndDa2Delta,
	GatItemRampEndDa2EndVal,
	GatItemRampEndDa2TLen,





	MaxNumOfGatItems = 255 //NOTE: Don't exceed this value when adding items
} GatItemNo;

typedef enum GatheringDataType : unsigned int
{
	GatUInt4Data = 0,
	GatInt4Data,
	GatUInt8Data,
	GatReal4Data,
	GatReal8Data,
	GatFix8Data,

	NumOfGatDataType
} GatDataType;

#endif /* SRC_COMMON_SYSTEMDEF_H_ */
