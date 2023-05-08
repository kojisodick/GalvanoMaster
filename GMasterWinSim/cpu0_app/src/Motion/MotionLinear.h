/*
 * MotionLinear.h
 *
 *  Created on: 2020/07/20
 *      Author: imao_yusuke
 */

#ifndef SRC_MOTION_MOTIONLINEAR_H_
#define SRC_MOTION_MOTIONLINEAR_H_

#include "Motion/Motion.h"

class MotionLinear: public Motion
{
public:
	MotionLinear();
	virtual ~MotionLinear();

	Motion * jumpAbs(int4 x, int4 y, real8 count=0);
	Motion * jumpAbs(int4 x, int4 y, int4 z, real8 count=0);
	Motion * jumpRel(int4 x, int4 y, real8 count=0);
	Motion * jumpRel(int4 x, int4 y, int4 z, real8 count=0);

	Motion * markAbs(int4 x, int4 y, real8 count=0);
	Motion * markAbs(int4 x, int4 y, int4 z, real8 count=0);
	Motion * markRel(int4 x, int4 y, real8 count=0);
	Motion * markRel(int4 x, int4 y, int4 z, real8 count=0);

	Motion * jumpSw(int4 x, int4 y, int4 z);
	Motion * markSw(int4 x, int4 y, int4 z);

	int4 run(bool enter=false);

	real8 arrivalCount;

private:
	void convertCoordinate();

	PlBlockLinearMove m_block;
};

#endif /* SRC_MOTION_MOTIONLINEAR_H_ */
