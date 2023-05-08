/*
 * MotionWait.h
 *
 *  Created on: 2020/03/16
 *      Author: imao_yusuke
 */

#ifndef SRC_MOTION_MOTIONWAIT_H_
#define SRC_MOTION_MOTIONWAIT_H_

#include <Motion/Motion.h>

class MotionWait: public Motion
{
public:
	MotionWait();
	~MotionWait();

	Motion * wait(uint4 count);


private:
	int4 run(bool enter=false);

	uint4 m_waitCount;

	PlBlockWait m_block;
};

#endif /* SRC_MOTION_MOTIONWAIT_H_ */
