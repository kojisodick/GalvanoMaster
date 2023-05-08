/*
 * MotionControl.cpp
 *
 *  Created on: 2020/07/28
 *      Author: imao_yusuke
 */

#include <Motion/MotionControl.h>

MotionControl::MotionControl()
{
	type = Begin;
}

MotionControl::~MotionControl()
{
	// TODO Auto-generated destructor stub
}

Motion * MotionControl::begin()
{
	m_code = BeginOfList;
	type = Begin;

	m_block.cmd = PlBlockListControl::CmdBeginList;

	return this;
}

Motion * MotionControl::end()
{
	m_code = EndOfList;
	type = End;

	m_block.cmd = PlBlockListControl::CmdEndList;

	return this;
}

Motion * MotionControl::pause(uint4 num)
{
	m_code = PauseOfList;
	m_pauseNumber = num;

	m_block.cmd = PlBlockListControl::CmdPauseList;
	return this;
}

int4 MotionControl::run(bool enter)
{
	int4 err = gp_plReg->setBlock((PlBlock *)&m_block);

	if( err ) {
		return err;
	}

//	if( enter ) {
//		return gp_plReg->setEnterRequest();
//	}

	return NoError;
}
