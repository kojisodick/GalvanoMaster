/*
 * Gathering.cpp
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#include <Component/Gathering.h>

#include "PL/PlRegister.h"
extern PlRegister *gp_plReg;

Gathering::Gathering(): m_enabled(false)
{
	// TODO Auto-generated constructor stub

}

Gathering::~Gathering()
{
	// TODO Auto-generated destructor stub
}

int4 Gathering::enable()
{
	m_enabled = true;

	return gp_plReg->setGatherEnableRequest();
}

int4 Gathering::disable()
{
	m_enabled = false;

	return gp_plReg->setGatherDisableRequest();
}

int4 Gathering::start()
{
	if( !m_enabled ){
		return -1;
	}

	return NoError;
}

int4 Gathering::stop()
{
	if( !m_enabled ){
		return -1;
	}

	return NoError;
}


Component * Gathering::setTrigger(uint4 val)
{
	m_block.code = PlBlock::CodeGatherControl;

	if( !m_enabled ) {
		m_block.trigger = 0;
	}
	else {
		m_block.trigger = (val == 1) ? 1 : 0;
	}

	m_baseBlock = (PlBlock *)&m_block;
	return this;
}
