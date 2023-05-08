/*
 * Component.h
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#ifndef SRC_COMPONENT_COMPONENT_H_
#define SRC_COMPONENT_COMPONENT_H_

#include "Common.h"
#include "PL/PlBlockStructure.h"
#include <Logger/Logger.h>
extern Logger *gp_log;

class Component
{
public:
	Component();
	virtual ~Component();

	virtual int4 run(bool enter=false);

protected:
	static int4 m_error;
	static PlBlock *m_baseBlock;
};

#endif /* SRC_COMPONENT_COMPONENT_H_ */
