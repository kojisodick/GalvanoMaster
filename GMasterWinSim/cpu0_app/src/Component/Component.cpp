/*
 * Component.cpp
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#include <Component/Component.h>

#include "PL/PlRegister.h"
extern PlRegister *gp_plReg;

int4      Component::m_error     = NoError;
PlBlock * Component::m_baseBlock = NULL;

Component::Component()
{
	// TODO Auto-generated constructor stub

}

Component::~Component()
{
	// TODO Auto-generated destructor stub
}

int4 Component::run(bool enter)
{
	m_error = gp_plReg->setBlock(m_baseBlock);

	if( m_error ) {
		return m_error;
	}

	if( enter ) {
		return gp_plReg->setEnterRequest();
	}

	return NoError;
}
