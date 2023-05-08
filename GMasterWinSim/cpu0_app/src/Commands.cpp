/*
 * Commands.cpp
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#include <Commands.h>
#include <Logger/Logger.h>
#include "ListBoxManager.h"

#ifdef WIN_MASTER_SIMULATOR
extern void start_new_run();
extern void finish_simulation();
#endif

extern Logger *gp_log;
extern ListBoxManager *gp_listManager;

Commands * Commands::obj()
{
	static Commands obj;
	return &obj;
}

//---------------------------------------------------------------------
Commands::Commands()
:m_skyWriting(&m_skyWritingMode3)
{
	m_skyWriting->setScannerObj(&m_scanner);
}

//---------------------------------------------------------------------
void Commands::init()
{
    for(int i=0;i<MAX_NUM_OF_GALVANO_CMD;i++){
    	cmd_func_ptr[i] = &Commands::command_0x00;
    }

    #define CMDFNCP(no)   cmd_func_ptr[no] = &Commands::command_##no

    //Control Commands
                   CMDFNCP(0x01); CMDFNCP(0x02); CMDFNCP(0x03);
    CMDFNCP(0x04); CMDFNCP(0x05); CMDFNCP(0x06); CMDFNCP(0x07);
    CMDFNCP(0x08); CMDFNCP(0x09); CMDFNCP(0x0a); CMDFNCP(0x0b);
    CMDFNCP(0x0c); CMDFNCP(0x0d); CMDFNCP(0x0e); CMDFNCP(0x0f);

    CMDFNCP(0x10); CMDFNCP(0x11); CMDFNCP(0x12); CMDFNCP(0x13);
    CMDFNCP(0x14); CMDFNCP(0x15); CMDFNCP(0x16); CMDFNCP(0x17);
    CMDFNCP(0x18); CMDFNCP(0x19); CMDFNCP(0x1a); CMDFNCP(0x1b);
    CMDFNCP(0x1c); CMDFNCP(0x1d); CMDFNCP(0x1e); CMDFNCP(0x1f);

    CMDFNCP(0x20); CMDFNCP(0x21); CMDFNCP(0x22); CMDFNCP(0x23);
    CMDFNCP(0x24); CMDFNCP(0x25); CMDFNCP(0x26); CMDFNCP(0x27);
    CMDFNCP(0x28); CMDFNCP(0x29); CMDFNCP(0x2a); CMDFNCP(0x2b);
    CMDFNCP(0x2c); CMDFNCP(0x2d); CMDFNCP(0x2e); CMDFNCP(0x2f);

    CMDFNCP(0x30); CMDFNCP(0x31); CMDFNCP(0x32); CMDFNCP(0x33);
    CMDFNCP(0x34); CMDFNCP(0x35); CMDFNCP(0x36); CMDFNCP(0x37);
    CMDFNCP(0x38); CMDFNCP(0x39); CMDFNCP(0x3a); CMDFNCP(0x3b);
    CMDFNCP(0x3c); CMDFNCP(0x3d); CMDFNCP(0x3e); CMDFNCP(0x3f);

    CMDFNCP(0x40); CMDFNCP(0x41); CMDFNCP(0x42); CMDFNCP(0x43);
    CMDFNCP(0x44); CMDFNCP(0x45); CMDFNCP(0x46); CMDFNCP(0x47);

    CMDFNCP(0x50); CMDFNCP(0x51); CMDFNCP(0x52); CMDFNCP(0x53);
    CMDFNCP(0x54); CMDFNCP(0x55); CMDFNCP(0x56); CMDFNCP(0x57);
    CMDFNCP(0x58); CMDFNCP(0x59); CMDFNCP(0x5a); CMDFNCP(0x5b);
    CMDFNCP(0x5c); CMDFNCP(0x5d); CMDFNCP(0x5e); CMDFNCP(0x5f);

    CMDFNCP(0x60); CMDFNCP(0x61); CMDFNCP(0x62); CMDFNCP(0x63);
    CMDFNCP(0x64); CMDFNCP(0x65); CMDFNCP(0x66); CMDFNCP(0x67);
    CMDFNCP(0x68); CMDFNCP(0x69); CMDFNCP(0x6a); CMDFNCP(0x6b);
    CMDFNCP(0x6c); CMDFNCP(0x6d); CMDFNCP(0x6e); CMDFNCP(0x6f);

    CMDFNCP(0x70); CMDFNCP(0x71); CMDFNCP(0x72); CMDFNCP(0x73);
    CMDFNCP(0x74); CMDFNCP(0x75); CMDFNCP(0x76); CMDFNCP(0x77);
    CMDFNCP(0x78); CMDFNCP(0x79); CMDFNCP(0x7a); CMDFNCP(0x7b);
    CMDFNCP(0x7c); CMDFNCP(0x7d); CMDFNCP(0x7e); CMDFNCP(0x7f);

    //List Commands
    CMDFNCP(0x80); CMDFNCP(0x81); CMDFNCP(0x82); CMDFNCP(0x83);
    CMDFNCP(0x84); CMDFNCP(0x85); CMDFNCP(0x86); CMDFNCP(0x87);
    CMDFNCP(0x88); CMDFNCP(0x89); CMDFNCP(0x8a); CMDFNCP(0x8b);
    CMDFNCP(0x8c); CMDFNCP(0x8d); CMDFNCP(0x8e); CMDFNCP(0x8f);

    CMDFNCP(0x90); CMDFNCP(0x91); CMDFNCP(0x92); CMDFNCP(0x93);
    CMDFNCP(0x94); CMDFNCP(0x95); CMDFNCP(0x96); CMDFNCP(0x97);
    CMDFNCP(0x98); CMDFNCP(0x99); CMDFNCP(0x9a); CMDFNCP(0x9b);
    CMDFNCP(0x9c); CMDFNCP(0x9d); CMDFNCP(0x9e); CMDFNCP(0x9f);

    CMDFNCP(0xa0); CMDFNCP(0xa1); CMDFNCP(0xa2); CMDFNCP(0xa3);
    CMDFNCP(0xa4); CMDFNCP(0xa5); CMDFNCP(0xa6); CMDFNCP(0xa7);
    CMDFNCP(0xa8); CMDFNCP(0xa9); CMDFNCP(0xaa); CMDFNCP(0xab);
    CMDFNCP(0xac); CMDFNCP(0xad); CMDFNCP(0xae); CMDFNCP(0xaf);

    CMDFNCP(0xb0); CMDFNCP(0xb1); CMDFNCP(0xb2); CMDFNCP(0xb3);
    CMDFNCP(0xb4); CMDFNCP(0xb5); CMDFNCP(0xb6); CMDFNCP(0xb7);

}

//---------------------------------------------------------------------
int4 Commands::execute(CommandData &cmd)
{
	if( cmd.code >= MAX_NUM_OF_GALVANO_CMD ){
		gp_log->error("Call invalid Command. (code=0x%x)", cmd.code);
		return CommandInvalidCode;
	}


	cmd.result = (uint1)((this->*cmd_func_ptr[cmd.code])(cmd.args));

	return NoError;
}
//---------------------------------------------------------------------
int4 Commands::run(CommandData &cmd)
{
	int4 err = CommandInvalidCode;
	if( cmd.code < LISTCMD_BASECODE ) {
		err = execute(cmd);
	}
	else {
		err = gp_listManager->addCommand(cmd);
	}

#if 1 //show received commands
	if( cmd.code != CmdCodeCtrlReadStatus && cmd.code != CmdCodeCtrlGetStatus
			&& cmd.code != CmdCodeCtrlGetErrInfo && cmd.code != CmdCodeCtrlRead4BitPort ) {
		gp_log->debug("cmd=0x%x", cmd.code);
	}
#endif

	return err;
}

//---------------------------------------------------------------------
int4 Commands::runListCommands()
{
	uint4 plState = gp_plReg->getPlStatus();

	if( plState == PlStsError ) {
		uint4 errNo = gp_plReg->getLatchedMasterError();
		if( errNo ) {
			return PLError;
		}
		return DriverError;
	}

	if( !gp_listManager->isRunning() ) {
		// PL & PS is Not Running
		return NoError;
	}

	int4 freeSpace = gp_plReg->getFreeSpaceInBlockArea();
	int4 err = NoError;
	CommandData cmd;

#ifdef WIN_MASTER_SIMULATOR
    start_new_run();
#endif

	while( freeSpace > 0 ){
		if( err ){
			gp_log->error("[Commands::runListCommands] Error has occurred. (err=%d)", err);
			break;
		}

		if( !m_skyWriting->isEmpty() ) {
			err = m_skyWriting->run();
			freeSpace--;
			continue;
		}

		if( !gp_listManager->isRunning() ) {
			gp_log->debug("[Commands::runListCommands] Complete. (list=%d, wIndex=%d, rIndex=%d)",
			gp_listManager->getReadCh(), gp_listManager->getWriteIndex(), gp_listManager->getReadIndex());
#ifdef WIN_MASTER_SIMULATOR
            finish_simulation();
#endif
            break;
		}

		err = gp_listManager->getCommand(cmd);

		if( !err ){
			if( cmd.code == CmdCodeListSetEndOfList ) {
				while( freeSpace < 5 ) {
					SLEEP_USEC(10);
					freeSpace = gp_plReg->getFreeSpaceInBlockArea();
				}
			}
			err = execute(cmd);
			freeSpace--;
		}

#ifdef DEBUG_PS_BLOCK
		freeSpace = 512;
#endif
	}

	return err;
}

//---------------------------------------------------------------------
/*!
 * Invalid Dummy command
 */
int4 Commands::command_0x00( CmdArgs &args )
{
	gp_log->error("Call invalid Command. (code=0x00)");
    return CommandInvalidCode;
}
//---------------------------------------------------------------------
