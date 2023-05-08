/*
 * MotionArc.h
 *
 *  Created on: 2020/07/20
 *      Author: imao_yusuke
 */

#ifndef SRC_MOTION_MOTIONARC_H_
#define SRC_MOTION_MOTIONARC_H_

#include <Motion/Motion.h>

class MotionArc: public Motion
{
public:
	MotionArc();
	virtual ~MotionArc();

	Motion * moveAbs(int4 cx, int4 cy, real8 angle_degree);
	Motion * moveRel(int4 cx, int4 cy, real8 angle_degree);

	Motion * moveSw(int4 *cmdPos, int4 *centerPos, real8 angle);

	int4  centerPos[MAX_NUM_OF_AXIS];
	real8 angle;                       //[radian]
	real8 radius;

	int4 run(bool enter=false);

private:
	void convertCoordinate();

	PlBlockArcMove m_block;
};

#endif /* SRC_MOTION_MOTIONARC_H_ */
