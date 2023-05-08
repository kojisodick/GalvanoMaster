/*
 * ShmCmdManager.cpp
 *
 *  Created on: 2018/12/21
 *      Author: imao_yusuke
 */

#include "ShmCmdManager1.h"
#include "ShmCommand.h"
#include "SharedMemory1.h"
#include "Gathering1.h"

extern SharedMemory1 *gp_sharedMem;
extern Gathering1    *gp_gather;

int4 dummyFunc(SCmdArgs &args);
int4 setGatheringSignal(SCmdArgs &args);
int4 setGatheringConfig(SCmdArgs &args);
int4 startGather(SCmdArgs &args);
int4 endGather(SCmdArgs &args);
int4 waitForStartOfGather(SCmdArgs &args);
int4 waitForEndOfGather(SCmdArgs &args);
int4 uploadGatheringData(SCmdArgs &args);
int4 getGatheringSignal(SCmdArgs &args);
int4 getGatheringConfig(SCmdArgs &args);
int4 getGatheringStatus(SCmdArgs &args);

//------------------------------------------------------------------------
ShmCmdManager1::ShmCmdManager1()
{
	this->init();
}

void ShmCmdManager1::init()
{
	for(uint4 code=0; code<MaxNumOfShmCmd; ++code) {
		m_funcPtr[code] = &dummyFunc;
	}

	m_funcPtr[ShmCmdCodeSetGatSignal]      = &setGatheringSignal;
	m_funcPtr[ShmCmdCodeSetGatConfig]      = &setGatheringConfig;
	m_funcPtr[ShmCmdCodeStartGat]          = &startGather;
	m_funcPtr[ShmCmdCodeEndGat]            = &endGather;
	m_funcPtr[ShmCmdCodeUploadGatData]     = &uploadGatheringData;
	m_funcPtr[ShmCmdCodeWaitForStartOfGat] = &waitForStartOfGather;
	m_funcPtr[ShmCmdCodeWaitForEndOfGat]   = &waitForEndOfGather;

	m_funcPtr[ShmCmdCodeGetGatStatus]      = &getGatheringStatus;
	m_funcPtr[ShmCmdCodeGetGatSignal]      = &getGatheringSignal;
	m_funcPtr[ShmCmdCodeGetGatConfig]      = &getGatheringConfig;
}

void ShmCmdManager1::runCommand()
{
	if( gp_sharedMem->recvCmdRequest(m_command) != NoError ) {
		return;
	}

	if( m_command.code >= MaxNumOfShmCmd ) {
		m_command.error = ShmCmdErrInvalidCmdCode;
		gp_sharedMem->sendCmdResponse(m_command);
		return;
	}

	m_command.error = (ShmCommandError)(*m_funcPtr[m_command.code])(m_command.args);

	gp_sharedMem->sendCmdResponse(m_command);
}



//-----------------------------------------------------------------------------
int4 dummyFunc(SCmdArgs &args)
{
	return ShmCmdErrInvalidCmdCode;
}

int4 setGatheringSignal(SCmdArgs &args)
{
	return gp_gather->selectItem(args.ui4[0], (GatItemNo)args.ui4[1]);
}

int4 setGatheringConfig(SCmdArgs &args)
{
	return gp_gather->setConfig(args.ui4[0], args.ui4[1]);
}

int4 startGather(SCmdArgs &args)
{
	gp_gather->start();
	return NoError;
}

int4 endGather(SCmdArgs &args)
{
	gp_gather->stop();
	return NoError;
}

int4 uploadGatheringData(SCmdArgs &args)
{
	uint1 *dataAddr = (uint1 *)gp_gather->getDataAddr(args.ui4[0]);
	uint4 dataLen   = gp_gather->getNumOfData() * sizeof(real4);

	gp_sharedMem->uploadData(dataAddr, dataLen);

	return NoError;
}

int4 waitForStartOfGather(SCmdArgs &args)
{
	gp_gather->waitForStart();
	return NoError;
}

int4 waitForEndOfGather(SCmdArgs &args)
{
	gp_gather->waitForEnd();


	return NoError;
}


int4 getGatheringSignal(SCmdArgs &args)
{
	GatItemNo num = (GatItemNo)args.ui4[1];

	gp_gather->getSelectedItem(args.ui4[0], num);

	return NoError;
}

int4 getGatheringConfig(SCmdArgs &args)
{
	bool loop;
	gp_gather->getConfig(args.ui4[0], args.ui4[1], loop);
	args.ui4[2] = (uint4)loop;

	return NoError;
}

int4 getGatheringStatus(SCmdArgs &args)
{
	args.ui4[0] = gp_gather->isRunning();
	args.ui4[1] = gp_gather->bufferIsFull();
	args.ui4[2] = gp_gather->getNumOfData();

	return NoError;
}
