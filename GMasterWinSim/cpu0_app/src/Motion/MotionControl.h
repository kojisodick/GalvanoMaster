/*
 * MotionControl.h
 *
 *  Created on: 2020/07/28
 *      Author: imao_yusuke
 */

#ifndef SRC_MOTION_MOTIONCONTROL_H_
#define SRC_MOTION_MOTIONCONTROL_H_

#include <Motion/Motion.h>

class MotionControl: public Motion
{
public:
	MotionControl();
	virtual ~MotionControl();

	Motion * begin();
	Motion * end();
	Motion * pause(uint4 num=0);

	int4 run(bool enter=false);

private:
	enum Code
	{
		BeginOfList,
		EndOfList,
		PauseOfList,
	} m_code;
	uint4 m_pauseNumber;

	PlBlockListControl m_block;
};

#endif /* SRC_MOTION_MOTIONCONTROL_H_ */
