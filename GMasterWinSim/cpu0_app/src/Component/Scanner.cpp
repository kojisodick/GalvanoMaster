/*
 * Scanner.cpp
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#include <Component/Scanner.h>

Scanner::Scanner()
{
	m_afterJumpDelay = 0;
	m_afterMarkDelay = 0;
	m_polygonDelay   = 0;

	m_jumpSpeed = 20  * 3488 * 0.001;
	m_markSpeed = 0.7 * 3488 * 0.001;
	m_gotoSpeed = 20  * 3488 * 0.001;

}

Scanner::~Scanner()
{
	// TODO Auto-generated destructor stub
}

Component * Scanner::setDelay(uint4 jump, uint4 mark, uint4 polygon)
{
	m_afterJumpDelay = jump;
	m_afterMarkDelay = mark;
	m_polygonDelay   = polygon;

	setDelayBlock();

	return this;
}

Component * Scanner::setJumpSpeed(real8 value)
{
	m_jumpSpeed = value * 0.01;

	setSpeedBlock();

	return this;
}

Component * Scanner::setMarkSpeed(real8 value)
{
	m_markSpeed = value * 0.01;

	setSpeedBlock();
	return this;
}

Component * Scanner::setGotoSpeed(real8 value)
{
	m_gotoSpeed = value * 0.01;

	setSpeedBlock();
	return this;
}


/* ------------------------------------------------
 * Private Method
 */
void Scanner::setDelayBlock()
{
	m_blockDelay.jump2mark = m_afterJumpDelay;
	m_blockDelay.mark2jump = m_afterMarkDelay;
	m_blockDelay.mark2mark = m_polygonDelay;

	m_baseBlock = (PlBlock *)&m_blockDelay;
}

void Scanner::setSpeedBlock()
{
	m_blockSpeed.jumpSpeed = m_jumpSpeed;
	m_blockSpeed.markSpeed = m_markSpeed;
	m_blockSpeed.gotoSpeed = m_gotoSpeed;

	m_baseBlock = (PlBlock *)&m_blockSpeed;
}
