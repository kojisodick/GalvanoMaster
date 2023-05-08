/*
 * Dpram.h
 *
 *  Created on: 2018/12/13
 *      Author: imao_yusuke
 */

#ifndef SRC_FPGA_DPRAM_H_
#define SRC_FPGA_DPRAM_H_

#include "Common.h"

class Dpram {
public:
	void  init();

	uint4 getControlCycle(){ return plStatus.bit.controlCycle; }
	uint4 getGatFlag()     { return plStatus.bit.gatBusy;      }

private:
	volatile uint4 recvPsCommand;                       //0x0000
	volatile uint4 recvWriteIndex;                      //0x0004
	volatile uint4 unused0[30];                         //0x0008-0x007F

	volatile uint4 plVersion;                           //0x0080
	volatile uint4 phaseCycleCounter;                   //0x0084
	volatile uint4 GatheringFlag;                       //0x0088
	volatile uint4 unused1[29];                         //0x008C-0x00FF

	union PlStatus
	{
		uint4  word;
		struct
		{
			uint4 status       : 8; //0-7
			uint4 laserEnabled : 1; //8
			uint4 laserOutOn   : 1; //9
			uint4 unused       : 1; //10
			uint4 gatBusy      : 1; //11
			uint4 controlCycle : 4; //12-15
			uint4 cmdCount     : 8; //16-23
			uint4 activeCh     : 4; //24-27
			uint4 pcbId        : 3; //28-30
			uint4 simMode      : 1; //31
		}  bit;
	} volatile plStatus;                                //0x0100
	volatile uint4 ReadIndex;                           //0x0104
	volatile uint4 moveIndex;                           //0x0108
	volatile uint4 haltNumber;                          //0x010C
	volatile int4  plCommandPos_bit[MaxNumOfAxes];      //0x0110-0x011B
	volatile int4  plCommandPos_pulse[MaxNumOfAxes];    //0x011C-0x0127
	volatile int4  actualPos_pulse[MaxNumOfAxes];       //0x0128-0x0133
	volatile uint4 latchedFirstMotorErr[MaxNumOfAxes];  //0x0134-0x013F
	volatile uint4 latchedFirstPlErr;                   //0x0140
	volatile uint4 updatedLatestMotorErr[MaxNumOfAxes]; //0x0144-0x014F
	volatile uint4 updatedLatestPlErr;                  //0x0150
	volatile uint4 actualData[4];                       //0x0154-0x0163
	volatile uint4 actualStatus[4];                     //0x0164-0x0173
	volatile uint4 blockLength;                         //0x0174
	volatile uint4 blockCount;                          //0x0178
	volatile uint4 cableDelay;                          //0x017C
	volatile uint4 laserIoDout12ActOutVal;              //0x0180
	volatile uint4 laserIoDout16ActInOutVal;            //0x0184
	volatile uint4 laserIoDout4ActInOutVal;             //0x0188
	volatile uint4 laserIoDaActOutVal;                  //0x018C
	volatile uint4 unused2[28];                         //0x0190-0x01FF

	volatile uint4 BlockMode;
	volatile fix8  pulseWeight[MaxNumOfAxes];
	volatile fix8  focalLength;
	volatile fix8  mirrorDistance;
	volatile uint4 moveMode;
	volatile int4  psCommandPos[MaxNumOfAxes];
	volatile fix8  arrivalCounts;
	volatile uint4 arcCenterPosX;
	volatile uint4 arcCenterPosY;
	volatile uint4 rotationAngle;
	volatile uint4 waitCounts;
	volatile fix8  jumpSpeed;
	volatile fix8  markSpeed;
	volatile fix8  GotoSpeed;
	volatile uint4 jump2markDelay;
	volatile uint4 mark2markDelay;
	volatile uint4 mark2jumpDelay;
	volatile uint4 laserLogics;
	volatile uint4 laserHalfPeriod;
	volatile uint4 laserPulse1Width;
	volatile uint4 laserPulse2Width;
	volatile uint4 laserOnDelay;
	volatile uint4 laserOffDelay;
	volatile uint4 laserStbyHalfPeriod;
	volatile uint4 laserStbyPulseWidth;
	volatile uint4 laserIoEnabled;
	volatile uint4 laserIoDout12OutputVal;
	volatile uint4 laserIoDout16OutputVal;
	volatile uint4 laserIoDout4OutputVal;
	volatile uint4 laserIoDa1OtputVal;
	volatile uint4 laserIoDa2OutputVal;
	volatile uint4 RampStartDout12Cycle;
	volatile uint4 RampStartDout12Delta;
	volatile uint4 RampStartDout12EndVal;
	volatile uint4 RampStartDa1Cycle;
	volatile uint4 RampStartDa1Delta;
	volatile uint4 RampStartDa1EndVal;
	volatile uint4 RampStartDa2Cycle;
	volatile uint4 RampStartDa2Delta;
	volatile uint4 RampStartDa2EndVal;
	volatile uint4 RampEndDout12Cycle;
	volatile uint4 RampEndDout12Delta;
	volatile uint4 RampEndDout12EndVal;
	volatile uint4 RampEndDout12Tlen;
	volatile uint4 RampEndDa1Cycle;
	volatile uint4 RampEndDa1Delta;
	volatile uint4 RampEndDa1EndVal;
	volatile uint4 RampEndDa1Tlen;
	volatile uint4 RampEndDa2Cycle;
	volatile uint4 RampEndDa2Delta;
	volatile uint4 RampEndDa2EndVal;
	volatile uint4 RampEndDa2Tlen;
	volatile uint4 latticeInterval;

#ifdef __arm__
private:
#else
public:
#endif
	Dpram(){}
	~Dpram(){}
};

#endif /* SRC_FPGA_DPRAM_H_ */
