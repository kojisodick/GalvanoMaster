/*
 * PlRegister.h
 *
 *  Created on: 2020/03/18
 *      Author: imao_yusuke
 */

#ifndef SRC_PL_PLREGISTER_H_
#define SRC_PL_PLREGISTER_H_

#include "Common.h"
#include "PlDef.h"

struct PlBlock;

class PlRegister
{
public:
	~PlRegister();

	static PlRegister * obj();

	int4 setEnterRequest();
	int4 setHaltRequest();
	int4 setAckRequest();
	int4 setStopRequest();
	int4 setIndexClearRequest();
	int4 setStartCycleHaltRequest();
	int4 setEndCycleHaltRequest();
	int4 setPLInitRequest();
	int4 setGatherEnableRequest();
	int4 setGatherDisableRequest();
	int4 setSimModeRequest();
	int4 setAxisDisable(uint4 axis);

	int4 setBlock(PlBlock *block);

	int4 setCorrectionData(int4 *data);

	uint4 getPlVersion()    { return stsArea ? stsArea->plVersion : 0; }
	uint4 getDriverVersion(){ return stsArea ? stsArea->driverStatus[X].version : 0; }

	uint4 getPsWriteIndex() { return ctrlArea ? ctrlArea->psWriteIndex : 0; }
	uint4 getPlReadIndex()  { return stsArea ? stsArea->plReadIndex : 0; }
	uint4 getPlMoveIndex()  { return stsArea ? stsArea->plMoveIndex : 0; }

	uint4 getPlStatus()     { return stsArea ? stsArea->plStatus.status : 0; }
	uint4 getControlCycle() { return stsArea ? stsArea->plStatus.controlCycle : 0; }
	uint4 getBoardID()      { return stsArea ? stsArea->plStatus.boardId : 0; }

	int4  getCmdPosBit(uint4 axis)	  { return (stsArea==0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->cmdPosBit[axis]; }
	int4  getCmdPosPulse(uint4 axis) { return (stsArea==0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->cmdPosPulse[axis]; }
	int4  getActPosPulse(uint4 axis) {	return (stsArea==0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->actPosPulse[axis]; }

	uint4 getLatchedMasterError(){ return stsArea ? stsArea->latchedMasterErr : 0; }
	uint4 getLatestMasterError() { return stsArea ? stsArea->latestMasterErr : 0; }

	uint4 getLatchedDriverError(uint4 axis) { return (stsArea == 0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->latchedDriverErr[axis]; }
	uint4 getLatestDriverError(uint4 axis)  { return (stsArea == 0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->latestDriverErr[axis]; }
	uint4 getDriverStatus(uint4 axis)       { return (stsArea == 0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->driverStatus[axis].status; }
	uint4 getDriverAlarmCode2(uint4 axis)   { return (stsArea == 0 || axis >= MAX_NUM_OF_AXIS) ? 0 : stsArea->alarmCode2[axis]; }

	uint4 getXY2ActualStatus(uint4 port) { return (stsArea == 0 || port >= 4 ) ? 0 : stsArea->xy2ActualStatus[port]; }


	uint4 getLaserIOValue(uint4 port);

	uint4 getRawValue(uint4 type, uint4 offset);

	uint4 getFreeSpaceInBlockArea();
private:
	PlRegister();

	int4 waitPLActive();
	int4 executePSCommand(uint4 cmdCode);
	void showBlock(const PlBlock *block);

private:
	struct ControlArea // 4 x 32 = 128bytes (0x80) 
	{
		struct PSCommand
		{
			uint4 code   : 8;  //0-7
			uint4 count  : 8;  //8-15
			uint4 key    : 12; //16-27
			uint4 unused : 4;  //28-31
		} psCommand;
		uint4 psWriteIndex;
		uint4 psResurved[30];
	} *ctrlArea;

	struct StatusArea // 4 x 32 = 128bytes (0x80) 
	{

		uint4 plVersion;
		uint4 phaseCounter;
		uint4 plResurved0[30];

		union PLStatus {
			struct {
				uint4 status : 8;		//0-7
				uint4 laserActive : 1;	//8
				uint4 laserOn : 1;		//9
				uint4 unused : 1;		//10
				uint4 gatherBusy : 1;	//11
				uint4 controlCycle : 4; //12-15
				uint4 cmdCount : 8;		//16-23
				uint4 activeCh : 4;		//24-27
				uint4 boardId : 3;		//28-30
				uint4 simMode : 1;		//31
			};
			uint4 wd;
		} plStatus;
		uint4 plReadIndex;
		uint4 plMoveIndex;
		uint4 haltNumber;
		uint4 cmdPosBit[MAX_NUM_OF_AXIS];
		uint4 cmdPosPulse[MAX_NUM_OF_AXIS];
		uint4 actPosPulse[MAX_NUM_OF_AXIS];
		uint4 latchedDriverErr[MAX_NUM_OF_AXIS];
		uint4 latchedMasterErr;
		uint4 latestDriverErr[MAX_NUM_OF_AXIS];
		uint4 latestMasterErr;
		uint4 xy2ActualStatus[4];
		uint4 actualLaserDout12;
		uint2 actualLaserDin16;
		uint2 actualLaserDout16;
		uint2 actualLaserDin4;
		uint2 actualLaserDout4;
		uint2 actualLaserDA2;
		uint2 actualLaserDA1;
		uint4 xy2StatusId;
		struct DriverStatus
		{
			uint4 fixedData;
			uint4 status;
			uint4 alarmCode;
			uint2 motorTemperature;
			uint2 driverTemperature;
			uint4 version;
			uint4 estimatedPosition;
			uint4 estimatedVelocity;
			uint4 reserved;
		} driverStatus[MAX_NUM_OF_AXIS];

		uint4 alarmCode2[MAX_NUM_OF_AXIS];
		uint4 plResurved1[7];
	} volatile *stsArea;

	PlBlock *blockArea;
	int4    *crrctArea[MAX_NUM_OF_AXIS];
};

#endif /* SRC_PL_PLREGISTER_H_ */
