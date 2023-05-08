/*
 * MotionWait.cpp
 *
 *  Created on: 2020/03/16
 *      Author: imao_yusuke
 */

#include <Motion/MotionWait.h>

MotionWait::MotionWait()
{
	type = MoveWait;
	m_waitCount = 0;
}

MotionWait::~MotionWait()
{
	// TODO Auto-generated destructor stub
}

Motion * MotionWait::wait(uint4 count)
{
	m_waitCount = count;

	m_block.count = m_waitCount;

	return this;
}

int4 MotionWait::run(bool enter)
{
	int4 err = gp_plReg->setBlock((PlBlock *)&m_block);

	if( err ) {
		return err;
	}

	if( enter ) {
		return gp_plReg->setEnterRequest();
	}

	return NoError;

}
