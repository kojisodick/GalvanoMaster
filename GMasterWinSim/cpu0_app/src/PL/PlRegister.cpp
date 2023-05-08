/*
 * PlRegister.cpp
 *
 *  Created on: 2020/03/18
 *      Author: imao_yusuke
 */

#include <string.h>

#ifdef __arm__
#include "xparameters.h"
#endif

#include <PL/PlRegister.h>
#include "PL/PlBlockStructure.h"

#include "Logger/Logger.h"
extern Logger *gp_log;

#define MAX_COMMAND_COUNT  (0xFF)

#ifdef WIN_MASTER_SIMULATOR
void gui_move2line(double x, double y, int blockmode);
#endif

//static void showBlock(PlMotionBlock *block)
//{
//
//}



enum PSCommandCode
{
	PSCmdDummy            = 0x00,
	PSCmdProgramEnter     = 0x01,
	PSCmdProgramHalt      = 0x02,
	PSCmdProgramAck       = 0x03,
	PSCmdProgramStop      = 0x04,
	PSCmdIndexClear       = 0x05,
	PSCmdCycleHaltEnable  = 0x06,
	PSCmdCycleHaltDisable = 0x07,
	PSCmdPLInit           = 0x08,
	PSCmdGatherEnable     = 0x09,
	PSCmdGatherDisable    = 0x0a,
	PSCmdSimModeEnable    = 0xf0,
	PSCmdXAxisDisable     = 0xf1,
	PSCmdYAxisDisable     = 0xf2,
	PSCmdZAxisDisable     = 0xf3,
};

PlRegister * PlRegister::obj()
{
	static PlRegister obj;
	return &obj;
}

//---------------------------------------------------------------------
PlRegister::PlRegister()
{
#ifdef __arm__
	ctrlArea     = (ControlArea *)(XPAR_AXI_BRAM_CTRL_CONTROL_S_AXI_BASEADDR);
	stsArea      = (StatusArea  *)(XPAR_AXI_BRAM_CTRL_CONTROL_S_AXI_BASEADDR + 0x80);
	blockArea    = (PlBlock *)(XPAR_AXI_BRAM_CTRL_BLOCK_S_AXI_BASEADDR);
	crrctArea[X] = (int4    *)(XPAR_AXI_BRAM_CTRL_CORRECTION_X_S_AXI_BASEADDR);
	crrctArea[Y] = (int4    *)(XPAR_AXI_BRAM_CTRL_CORRECTION_Y_S_AXI_BASEADDR);
	crrctArea[Z] = (int4    *)(XPAR_AXI_BRAM_CTRL_CORRECTION_Z_S_AXI_BASEADDR);
#else
	ctrlArea = 0;
	static StatusArea objStatus;
	stsArea = &objStatus;
	blockArea = 0;
	crrctArea[X] = 0;
	crrctArea[Y] = 0;
	crrctArea[Z] = 0;
#endif
}

/* --------------------------------------------------------------------
 *
 */
PlRegister::~PlRegister()
{
	// TODO Auto-generated destructor stub
}


/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setEnterRequest()
{
	int4 err = waitPLActive();
	if( err ) {
		return err;
	}

	return executePSCommand(PSCmdProgramEnter);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setHaltRequest()
{
//	uint4 status = stsArea->plStatus.status;
//	if( status != PlStsBusy) {
//		return ;
//	}

	return executePSCommand(PSCmdProgramHalt);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setAckRequest()
{

	uint4 status = stsArea->plStatus.status;
	if( status != PlStsError) {
		gp_log->debug("PL_ACK requested but failed due to status mismatch(0x%x).", status);
		return PLInvalidStatus; // FIXME
	}

#ifdef __arm__
	ctrlArea->psWriteIndex = 0;
#endif
	return executePSCommand(PSCmdProgramAck);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setStopRequest()
{
//	uint4 status = stsArea->plStatus.status;
//	if( status != PlStsBusy) {
//		return;
//	}

	return executePSCommand(PSCmdProgramStop);

}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setIndexClearRequest()
{
	return NoError; //NOTE: Do not use

	return executePSCommand(PSCmdIndexClear);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setStartCycleHaltRequest()
{
	return NoError; //NOTE: Do not use

	return executePSCommand(PSCmdCycleHaltEnable);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setEndCycleHaltRequest()
{
	return NoError; //NOTE: Do not use

	return executePSCommand(PSCmdCycleHaltDisable);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setPLInitRequest()
{

	uint4 status = stsArea->plStatus.status;
//	if( status != PlStsBoot ) {
//		gp_log->debug("-> PL_INIT has already completed. (sts=0x%x)", status);
//		return NoError;
//	}

	int4 err = executePSCommand(PSCmdPLInit);

	if( err ) {
		return err;
	}

	uint4 counter = 0;
	while(1) {
		status = stsArea->plStatus.status;
		if( status == PlStsActive ) {
			gp_log->debug("-> PL_INIT successful. (time=%u usec)", counter*100);
			break;
		}

		if( ++counter >= 10000 ) {
			gp_log->error("-> PL_INIT timeout. (cmdCnt=%u, sts=0x%x)", stsArea->plStatus.cmdCount,  status);
			gp_log->error("-> XY2Status(0x%x, 0x%x, 0x%x)",
					stsArea->xy2ActualStatus[0],
					stsArea->xy2ActualStatus[1],
					stsArea->xy2ActualStatus[2]
					);

			return PLInitFailed; //FIXME
		}
		SLEEP_USEC(100);
	}

	return NoError;
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setGatherEnableRequest()
{
	int4 err = waitPLActive();
	if( err ) {
		return err;
	}

	err = executePSCommand(PSCmdGatherEnable);
#if 1
	SLEEP_USEC(300);  //NOTE: wait for XY2 status change
#endif
	return NoError;
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setGatherDisableRequest()
{
	int4 err = waitPLActive();
	if( err ) {
		return err;
	}

	err = executePSCommand(PSCmdGatherDisable);
#if 1
	SLEEP_USEC(300); //NOTE: wait for XY2 status change
#endif
	return NoError;
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setSimModeRequest()
{
	return executePSCommand(PSCmdSimModeEnable);
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setAxisDisable(uint4 axis)
{
	if( axis >= MAX_NUM_OF_AXIS ) {
		return -1; //FIXME: not reach
	}

	return executePSCommand(PSCmdXAxisDisable + axis);

}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setBlock(PlBlock *block)
{
#ifdef __arm__
	uint4 writeIndex = ctrlArea->psWriteIndex;
	uint4 readIndex  = stsArea->plReadIndex;
	int8  diff       = writeIndex - readIndex;

#ifdef DEBUG_PS_BLOCK
	if(block->code == 0x5) {
		showBlock(block);
	}
#else
	int4 err = NoError;
	if( diff > 0 ) {
		if( diff >= 512 ){
			err = PLIsBusy;
		}
	}
	else {
		if( diff + 0xFFFFFFFF >= 512 ){
			err = PLIsBusy;
		}
	}

	if( err ) {
		gp_log->error("[PlRegister::setBlock] Buffer is full. (wIndex=%u, rIndex=%u)", writeIndex, readIndex);
		return err;
	}

	memcpy(&blockArea[writeIndex&0x1FF], block, sizeof(PlBlock));
#endif
	ctrlArea->psWriteIndex = (writeIndex==0xFFFFFFFF) ? 0 : writeIndex+1;
#else // for Windows Simulator
	// TODO : Output to a file?
	// 

	switch (block->code) {
	case PlBlock::CodeLinearMove :
	{
		PlBlockLinearMove * linear = (PlBlockLinearMove *)block;
        gui_move2line((double)linear->cmdPos[0],(double)linear->cmdPos[1], linear->mode);
		//printf("LNR,%d,%d\n",linear->cmdPos[0], linear->cmdPos[1]);
		break;
	}
	case PlBlock::CodeArcMove :
	{
		PlBlockArcMove * arc = (PlBlockArcMove *)block;
        //gui_move2line((double)arc->cmdPos[0], (double)arc->cmdPos[1]);
        printf("ARC,%d,%d\n", arc->cmdPos[0], arc->cmdPos[1]);

		break;
	}
	default :
		break;
	}
#endif
	return NoError;
}

/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::setCorrectionData(int4 *data)
{
	if( !data ) {
		return -1; //FIXME: not reach
	}

#ifdef __arm__
	const uint4 row  = 128;
	const uint4 line = 128;
	uint4 offset = 0;

	for(uint4 axis=0; axis<MAX_NUM_OF_AXIS; ++axis) {
		memcpy(crrctArea[axis], &data[offset], (sizeof(int4)*row * line));
		offset += (row * line);
	}
#endif
	return NoError;
}

/* --------------------------------------------------------------------
 *
 */
uint4 PlRegister::getLaserIOValue(uint4 port)
{
	uint4 res = 0;

#ifdef __arm__
	if( port == LaserPortDout12 )      res = stsArea->actualLaserDout12;
	else if( port == LaserPortDout16 ) res = stsArea->actualLaserDout16;
	else if( port == LaserPortDin16  ) res = stsArea->actualLaserDin16;
	else if( port == LaserPortDout4  ) res = stsArea->actualLaserDout4;
	else if( port == LaserPortDin4   ) res = stsArea->actualLaserDin4;
	else if( port == LaserPortDA1    ) res = stsArea->actualLaserDA1 >> 4;
	else if( port == LaserPortDA2    ) res = stsArea->actualLaserDA2 >> 4;
#endif
	return res;
}

/* --------------------------------------------------------------------
 *
 */
uint4 PlRegister::getRawValue(uint4 type, uint4 offset)
{
#ifdef __arm__
	uint4 *addr = NULL;
	if( type == 0 )      addr = (uint4 *)(XPAR_AXI_BRAM_CTRL_CONTROL_S_AXI_BASEADDR);
	else if( type == 1 ) addr = (uint4 *)(XPAR_AXI_BRAM_CTRL_BLOCK_S_AXI_BASEADDR);
	else if( type == 2 ) addr = (uint4 *)(XPAR_AXI_BRAM_CTRL_CORRECTION_X_S_AXI_BASEADDR);
	else if( type == 3 ) addr = (uint4 *)(XPAR_AXI_BRAM_CTRL_CORRECTION_Y_S_AXI_BASEADDR);
	else if( type == 4 ) addr = (uint4 *)(XPAR_AXI_BRAM_CTRL_CORRECTION_Z_S_AXI_BASEADDR);

	if( !addr ) {
		return 0;
	}

	return *(addr + offset);
#else
	if (type == 0 && offset == 0x40) return (uint4)(stsArea->plStatus.wd);
	return 0;
#endif
}

/* --------------------------------------------------------------------
 *
 */
uint4 PlRegister::getFreeSpaceInBlockArea()
{
#ifdef __arm__
	uint4 wIndex = ctrlArea->psWriteIndex;
	uint4 rIndex = stsArea->plReadIndex;

	uint4 diff = ( wIndex > rIndex ) ? ( wIndex - rIndex )
			: ((0xFFFFFFFF - rIndex) + wIndex);

	return (512 - diff);
#else
	return 512;
#endif
}




/* --------------------------------------------------------------------
 *
 */
int4 PlRegister::executePSCommand(uint4 cmdCode)
{
#ifdef __arm__
	ControlArea::PSCommand tmp = ctrlArea->psCommand;
	tmp.code   = cmdCode & 0xFF;
	tmp.count  = (tmp.count == MAX_COMMAND_COUNT) ? 0 : tmp.count+1;
	tmp.key    = 0;
	tmp.unused = 0;

	ctrlArea->psCommand = tmp;

	uint4 timeout=0;
	while(1) {
		SLEEP_USEC(10); // wait 10usec

		if( stsArea->plStatus.cmdCount == tmp.count ) {
			break;
		}

		if( ++timeout >= 1000 ) {
			gp_log->error("[PlRegister::executePSCommand] timeout! (code=0x%x)", cmdCode);
			return PLIsBusy;
		}
	}
#else
	// ONLY for Windows simulator 
	switch (cmdCode) {
	case PSCmdPLInit :
		stsArea->plStatus.status = PlStsActive;
		break;
	case PSCmdProgramStop:
		stsArea->plStatus.status = PlStsError;
		break;
	case PSCmdProgramAck:
		if(stsArea->plStatus.status == PlStsError)
			stsArea->plStatus.status = PlStsActive;
		break;
	case PSCmdProgramEnter :
		if (stsArea->plStatus.status == PlStsActive)
			stsArea->plStatus.status = PlStsActive;
		break;
	case PSCmdSimModeEnable :
		stsArea->plStatus.simMode = 1;
		break;
	default :
		break;
	}
#endif
	return NoError;
}

int4 PlRegister::waitPLActive()
{
//#ifdef __arm__
	uint4 counter = 0;
	uint4 sts;
	while( (sts = stsArea->plStatus.status) == PlStsBusy ) {
		if( counter > 1000 ) {
			gp_log->error("[PlRegister::waitPLActive] timeout! (sts=0x%x)", sts);
			return PLIsBusy;
		}
		SLEEP_USEC(10);
		counter++;
	}
//#endif
	return NoError;
}


void PlRegister::showBlock(const PlBlock *block)
{

	gp_log->error("-> Block[%u]:code=%x,%x,%x,%x,%x,%x,%x,%x",
#ifdef __arm__
		    ctrlArea->psWriteIndex,
#else
			0,
#endif
		    block->code,
			block->param[0],
			block->param[1],
			block->param[2],
			block->param[3],
			block->param[4],
			block->param[5],
			block->param[6]);
}

