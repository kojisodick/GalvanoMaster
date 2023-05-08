/*
 * Gathering.h
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#ifndef SRC_COMPONENT_GATHERING_H_
#define SRC_COMPONENT_GATHERING_H_

#include <Component/Component.h>

class Gathering: public Component
{
public:
	Gathering();
	virtual ~Gathering();

	int4 enable();
	int4 disable();

	int4 start();
	int4 stop();

	Component * setTrigger(uint4 val);

private:
	bool m_enabled;

	PlBlockGatherControl m_block;
};

#endif /* SRC_COMPONENT_GATHERING_H_ */
