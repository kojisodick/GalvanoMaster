/*
 * PlDef.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMON_PLDEF_H_
#define SRC_COMMON_PLDEF_H_

enum LaserIOPortNo:unsigned long
{
	LaserPortDout12 = 0,
	LaserPortDout16,
	LaserPortDin16,
	LaserPortDout4,
	LaserPortDin4,
	LaserPortDA1,
	LaserPortDA2,
};

enum PlListControlCode :unsigned long
{
	ListCodeListHalt  = 0x00,
	ListCodeListStart = 0x01,
	ListCodeListEnd   = 0x02,
	ListCodeInvalid   = 0xFF
};

enum PlMoveMode : unsigned long
{
	MoveModeVsc     = 0x00,
	MoveModeMark    = 0x01,
	MoveModeJump    = 0x10,
};

enum PlStatus: unsigned long
{
	PlStsBoot    = 0x00,
	PlStsInit    = 0x01,
	PlStsWait    = 0x02,
	PlStsActive  = 0x03,
	PlStsBusy    = 0x04,
	PlStsHalt    = 0x05,
	PlStsFormat  = 0x06,
	PlStsError   = 0xFF,
};

enum PlErrorNumber: unsigned long
{
	PlErrNoError                  = 0x0000,
	PlErrAxisNotFound	          = 0x0001,
	PlErrPlWatchDog               = 0x0002,
	PlErrStopCommand              = 0x0004,
	PlErrRuntimeError             = 0x0008,
	PlErrInvalidStandbyLaserParam = 0x0010,
	PlErrInvalidLaserParam        = 0x0020,
	PlErrInvalidLaserMode         = 0x0040,
	PlErrStockEmpty               = 0x0080,
};





enum PsCommandId: unsigned long
{
	PsCmdDummy           = 0x00,
	PsCmdEnter           = 0x01,
	PsCmdBlockHalt       = 0x02,
	PsCmdAck             = 0x03,
	PsCmdStop            = 0x04,
	PsCmdIndexClear      = 0x05,
	PsCmdCycleHaltStart  = 0x06,
	PsCmdCycleHaltEnd    = 0x07,
	PsCmdInit            = 0x08,
	PsCmdGatherEnable    = 0x09,
	PsCmdGatherDisable   = 0x0a,
	PsCmdSimMode         = 0xf0,
};

enum ScannerCmdTargetCh: unsigned long
{
	ScnrCmdCh1 = 0x1,
	ScnrCmdCh2 = 0x2,
	ScnrCmdCh3 = 0x4,
	ScnrCmdCh4 = 0x8,
};

enum ScannerCmdId
{
	ScnrCmdDummy         = 0x00,
	ScnrCmdEmergencyStop = 0x01,
	ScnrCmdCalibration   = 0x02,
	ScnrCmdAlarmClear    = 0x03,
	ScnrCmdChangeStatus  = 0x10,
};

enum ScannerCmdData: unsigned long
{
	ScnrCmdDataOff = 0x00000,
	ScnrCmdDataOn  = 0x00001,
};


enum ScannerStatusId: unsigned long
{
	ScnrStsDummy                  = 0x00,
	ScnrStsFixedData              = 0x01,
	ScnrStsDriverSts              = 0x02,
	ScnrStsAlarmCode              = 0x03,
	ScnrStsMotorThermistor        = 0x04,
	ScnrStsDriverThermistor       = 0x05,
	ScnrStsCurrentPos             = 0x06,
	ScnrStsFirmwareVersion        = 0x07,
	ScnrStsCurrentPosAndAlarm     = 0x09,
	ScnrStsEstimatedPosAndAlarm   = 0x0A,
	ScnrStsEstimatedSpeedAndAlarm = 0x0B
};

enum LaserActive: unsigned long
{
	LaserDisabled = 0,
	LaserEnabled  = 1,
};

enum LaserOutput: unsigned long
{
	LaserOutOff = 0,
	LaserOutOn  = 1,
};

#endif /* SRC_COMMON_PLDEF_H_ */
