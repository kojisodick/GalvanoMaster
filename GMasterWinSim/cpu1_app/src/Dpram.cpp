/*
 * Dpram.cpp
 *
 *  Created on: 2018/12/13
 *      Author: imao_yusuke
 */

#include <Dpram.h>

#include "Gathering1.h"

extern Gathering1 *gp_gather;

uint4 fix8Data(const void *src);

void Dpram::init()
{
	gp_gather->registerToItemList(GatItemPsCommand        , (const void *)&recvPsCommand            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemWriteIndex       , (const void *)&recvWriteIndex           , dataFormat<uint4> );

	gp_gather->registerToItemList(GatItemPlVersion        , (const void *)&plVersion                , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemPhaseCounter     , (const void *)&phaseCycleCounter        , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemGatheringFlag    , (const void *)&GatheringFlag            , dataFormat<uint4> );

	gp_gather->registerToItemList(GatItemPlStatus         , (const void *)&plStatus.word            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemReadIndex        , (const void *)&ReadIndex                , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemMoveIndex        , (const void *)&moveIndex                , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemHaltNumber       , (const void *)&haltNumber               , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemCmdPosBit_X      , (const void *)&plCommandPos_bit[X]      , dataFormat<int4>  );
	gp_gather->registerToItemList(GatItemCmdPosBit_Y      , (const void *)&plCommandPos_bit[Y]      , dataFormat<int4>  );
	gp_gather->registerToItemList(GatItemCmdPosBit_Z      , (const void *)&plCommandPos_bit[Z]      , dataFormat<int4>  );
	gp_gather->registerToItemList(GatItemCmdPosPulse_X    , (const void *)&plCommandPos_pulse[X]    , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemCmdPosPulse_Y    , (const void *)&plCommandPos_pulse[Y]    , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemCmdPosPulse_Z    , (const void *)&plCommandPos_pulse[Z]    , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActPosPulse_X    , (const void *)&actualPos_pulse[X]       , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActPosPulse_Y    , (const void *)&actualPos_pulse[Y]       , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActPosPulse_Z    , (const void *)&actualPos_pulse[Z]       , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemFirstMotorErr_X  , (const void *)&latchedFirstMotorErr[X]  , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemFirstMotorErr_Y  , (const void *)&latchedFirstMotorErr[Y]  , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemFirstMotorErr_Z  , (const void *)&latchedFirstMotorErr[Z]  , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemFirstPlErr       , (const void *)&latchedFirstPlErr        , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemLatestMotorErr_X , (const void *)&updatedLatestMotorErr[X] , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemLatestMotorErr_Y , (const void *)&updatedLatestMotorErr[Y] , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemLatestMotorErr_Z , (const void *)&updatedLatestMotorErr[Z] , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemLatestPlErr      , (const void *)&updatedLatestPlErr       , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActData1         , (const void *)&actualData[0]            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActData2         , (const void *)&actualData[1]            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActData3         , (const void *)&actualData[2]            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActData4         , (const void *)&actualData[3]            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActStatus0       , (const void *)&actualStatus[0]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActStatus1       , (const void *)&actualStatus[1]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActStatus2       , (const void *)&actualStatus[2]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActStatus3       , (const void *)&actualStatus[3]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockLength      , (const void *)&blockLength              , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockCount       , (const void *)&blockCount               , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemCableDelay       , (const void *)&cableDelay               , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActualLaserDout12Val , (const void *)&laserIoDout12ActOutVal   , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActualLaserDio16Val  , (const void *)&laserIoDout16ActInOutVal , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActualLaserDio4AVal  , (const void *)&laserIoDout4ActInOutVal  , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemActualLaserDa1Da2Val , (const void *)&laserIoDaActOutVal       , dataFormat<uint4> );

	gp_gather->registerToItemList(GatItemBlockMode        , (const void *)&BlockMode                , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemPulseWeight_X    , (const void *)&pulseWeight[X]           , fix8Data          );
	gp_gather->registerToItemList(GatItemPulseWeight_Y    , (const void *)&pulseWeight[Y]           , fix8Data          );
	gp_gather->registerToItemList(GatItemPulseWeight_Z    , (const void *)&pulseWeight[Z]           , fix8Data          );
	gp_gather->registerToItemList(GatItemFocalLength      , (const void *)&focalLength              , fix8Data          );
	gp_gather->registerToItemList(GatItemMirrorDistance   , (const void *)&mirrorDistance           , fix8Data          );
	gp_gather->registerToItemList(GatItemBlockMovemode    , (const void *)&moveMode                 , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockCmdPos_X    , (const void *)&psCommandPos[X]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockCmdPos_Y    , (const void *)&psCommandPos[Y]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockCmdPos_Z    , (const void *)&psCommandPos[Z]          , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockArrivalCnt  , (const void *)&arrivalCounts            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockCenterPos_X , (const void *)&arcCenterPosX            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockCenterPos_Y , (const void *)&arcCenterPosY            , dataFormat<uint4> );
	gp_gather->registerToItemList(GatItemBlockArcAngle    , (const void *)&rotationAngle            , dataFormat<uint4> );
}
