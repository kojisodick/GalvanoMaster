/*
 * Scanner.h
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#ifndef SRC_COMPONENT_SCANNER_H_
#define SRC_COMPONENT_SCANNER_H_

#include <Component/Component.h>

class Scanner: public Component
{
public:
	Scanner();
	virtual ~Scanner();

	Component * setDelay(uint4 jump, uint4 mark, uint4 polygon);

	Component * setJumpSpeed(real8 value);
	Component * setMarkSpeed(real8 value);
	Component * setGotoSpeed(real8 value);

	real8 getMarkSpeed(){ return m_markSpeed; }


private:
	void setDelayBlock();
	void setSpeedBlock();


	uint4 m_afterJumpDelay;
	uint4 m_afterMarkDelay;
	uint4 m_polygonDelay;

	real8 m_jumpSpeed;
	real8 m_markSpeed;
	real8 m_gotoSpeed;

	PlBlockDelayConf m_blockDelay;
	PlBlockSpeedConf m_blockSpeed;
};

#endif /* SRC_COMPONENT_SCANNER_H_ */
