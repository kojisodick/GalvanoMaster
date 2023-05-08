/*
 * Commands.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_COMMAND_COMMANDS_H_
#define SRC_COMMAND_COMMANDS_H_

#include "CommandData.h"
#include "SkyWriting/SkyWritingMode1.h"
#include "SkyWriting/SkyWritingMode2.h"
#include "SkyWriting/SkyWritingMode3.h"

#include "Motion/MotionLinear.h"
#include "Motion/MotionArc.h"
#include "Motion/MotionWait.h"
#include "Motion/MotionControl.h"

#include "Component/Gathering.h"
#include "Component/Laser.h"
#include "Component/LaserIO.h"
#include "Component/OpticalSystem.h"
#include "Component/Scanner.h"

#include "PL/PlRegister.h"
extern PlRegister *gp_plReg;

class Commands
{
public:
	static Commands * obj();
	~Commands(){}

	void init();

	int4 execute(CommandData &data);
	int4 run(CommandData &cmd);
	int4 runListCommands();

	void  stopRunning() { gp_plReg->setStopRequest(); }

	uint4 getControlCycle() { return gp_plReg->getControlCycle();}
	uint4 getBoardID()      { return gp_plReg->getBoardID();}




private:
	Commands();

	SkyWriting       m_skyWritingMode0;
	SkyWritingMode1  m_skyWritingMode1;
	SkyWritingMode2  m_skyWritingMode2;
	SkyWritingMode3  m_skyWritingMode3;
	SkyWriting      *m_skyWriting;

	MotionLinear     m_mtnLinear;
	MotionArc        m_mtnArc;
	MotionWait       m_mtnWait;
	MotionControl    m_mtnCtrl;

	Laser            m_laser;
	LaserIO          m_laserIO;
	Scanner          m_scanner;
	OpticalSystem    m_opticalSys;
	Gathering        m_gathering;


private:
	/* Galvano Commands */
	int4 (Commands::*cmd_func_ptr[MAX_NUM_OF_GALVANO_CMD])(CmdArgs &args);

#define CMDFNC(no)    int4 command_##no(CmdArgs &args)

	// Control
	CMDFNC(0x00); CMDFNC(0x01); CMDFNC(0x02); CMDFNC(0x03);
    CMDFNC(0x04); CMDFNC(0x05); CMDFNC(0x06); CMDFNC(0x07);
    CMDFNC(0x08); CMDFNC(0x09); CMDFNC(0x0a); CMDFNC(0x0b);
    CMDFNC(0x0c); CMDFNC(0x0d); CMDFNC(0x0e); CMDFNC(0x0f);

    CMDFNC(0x10); CMDFNC(0x11); CMDFNC(0x12); CMDFNC(0x13);
    CMDFNC(0x14); CMDFNC(0x15); CMDFNC(0x16); CMDFNC(0x17);
    CMDFNC(0x18); CMDFNC(0x19); CMDFNC(0x1a); CMDFNC(0x1b);
    CMDFNC(0x1c); CMDFNC(0x1d); CMDFNC(0x1e); CMDFNC(0x1f);

    CMDFNC(0x20); CMDFNC(0x21); CMDFNC(0x22); CMDFNC(0x23);
    CMDFNC(0x24); CMDFNC(0x25); CMDFNC(0x26); CMDFNC(0x27);
    CMDFNC(0x28); CMDFNC(0x29); CMDFNC(0x2a); CMDFNC(0x2b);
    CMDFNC(0x2c); CMDFNC(0x2d); CMDFNC(0x2e); CMDFNC(0x2f);

    CMDFNC(0x30); CMDFNC(0x31); CMDFNC(0x32); CMDFNC(0x33);
    CMDFNC(0x34); CMDFNC(0x35); CMDFNC(0x36); CMDFNC(0x37);
    CMDFNC(0x38); CMDFNC(0x39); CMDFNC(0x3a); CMDFNC(0x3b);
    CMDFNC(0x3c); CMDFNC(0x3d); CMDFNC(0x3e); CMDFNC(0x3f);

    CMDFNC(0x40); CMDFNC(0x41); CMDFNC(0x42); CMDFNC(0x43);
    CMDFNC(0x44); CMDFNC(0x45); CMDFNC(0x46); CMDFNC(0x47);

    CMDFNC(0x50); CMDFNC(0x51); CMDFNC(0x52); CMDFNC(0x53);
    CMDFNC(0x54); CMDFNC(0x55); CMDFNC(0x56); CMDFNC(0x57);
    CMDFNC(0x58); CMDFNC(0x59); CMDFNC(0x5a); CMDFNC(0x5b);
    CMDFNC(0x5c); CMDFNC(0x5d); CMDFNC(0x5e); CMDFNC(0x5f);

    CMDFNC(0x60); CMDFNC(0x61); CMDFNC(0x62); CMDFNC(0x63);
    CMDFNC(0x64); CMDFNC(0x65); CMDFNC(0x66); CMDFNC(0x67);
    CMDFNC(0x68); CMDFNC(0x69); CMDFNC(0x6a); CMDFNC(0x6b);
    CMDFNC(0x6c); CMDFNC(0x6d); CMDFNC(0x6e); CMDFNC(0x6f);

    CMDFNC(0x70); CMDFNC(0x71); CMDFNC(0x72); CMDFNC(0x73);
    CMDFNC(0x74); CMDFNC(0x75); CMDFNC(0x76); CMDFNC(0x77);
    CMDFNC(0x78); CMDFNC(0x79); CMDFNC(0x7a); CMDFNC(0x7b);
    CMDFNC(0x7c); CMDFNC(0x7d); CMDFNC(0x7e); CMDFNC(0x7f);


    // List
    CMDFNC(0x80); CMDFNC(0x81); CMDFNC(0x82); CMDFNC(0x83);
    CMDFNC(0x84); CMDFNC(0x85); CMDFNC(0x86); CMDFNC(0x87);
    CMDFNC(0x88); CMDFNC(0x89); CMDFNC(0x8a); CMDFNC(0x8b);
    CMDFNC(0x8c); CMDFNC(0x8d); CMDFNC(0x8e); CMDFNC(0x8f);

    CMDFNC(0x90); CMDFNC(0x91); CMDFNC(0x92); CMDFNC(0x93);
    CMDFNC(0x94); CMDFNC(0x95); CMDFNC(0x96); CMDFNC(0x97);
    CMDFNC(0x98); CMDFNC(0x99); CMDFNC(0x9a); CMDFNC(0x9b);
    CMDFNC(0x9c); CMDFNC(0x9d); CMDFNC(0x9e); CMDFNC(0x9f);

    CMDFNC(0xa0); CMDFNC(0xa1); CMDFNC(0xa2); CMDFNC(0xa3);
    CMDFNC(0xa4); CMDFNC(0xa5); CMDFNC(0xa6); CMDFNC(0xa7);
    CMDFNC(0xa8); CMDFNC(0xa9); CMDFNC(0xaa); CMDFNC(0xab);
    CMDFNC(0xac); CMDFNC(0xad); CMDFNC(0xae); CMDFNC(0xaf);

    CMDFNC(0xb0); CMDFNC(0xb1); CMDFNC(0xb2); CMDFNC(0xb3);
    CMDFNC(0xb4); CMDFNC(0xb5); CMDFNC(0xb6); CMDFNC(0xb7);
};

#endif /* SRC_COMMAND_COMMANDS_H_ */
