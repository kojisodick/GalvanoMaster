/*
 * Motion.cpp
 *
 *  Created on: 2020/03/13
 *      Author: imao_yusuke
 */

#include <Motion/Motion.h>




int4  Motion::startPos[MAX_NUM_OF_AXIS] = {0, 0, 0};
int4  Motion::endPos[MAX_NUM_OF_AXIS]   = {0, 0, 0};
real8 Motion::length = 0.0;

int4  Motion::m_offset[MAX_NUM_OF_AXIS]        = {0, 0, 0};
real8 Motion::m_magnification[MAX_NUM_OF_AXIS] = {1.0, 1.0, 1.0};
real8 Motion::m_rotationAngle = 0.0;


Motion::Motion()
{

}

int4 Motion::setOffset(uint4 axis, int4 value)
{
	if( axis >= MAX_NUM_OF_AXIS ) {
		return -1;
	}

	m_offset[axis] = value;

//	gp_log->debug(" -> set Offset[%d]=%d", axis, value);

	return NoError;
}

int4 Motion::setMagnification(uint4 axis, real8 value)
{
	if( axis >= MAX_NUM_OF_AXIS ) {
		return -1;
	}

	m_magnification[axis] = value;

	gp_log->debug(" -> set Magnification[%d]=%f", axis, value);
	return NoError;
}

int4 Motion::setRotationAngle(real8 value)
{
	m_rotationAngle = value;

	gp_log->debug(" -> set RotationAngle=%f", value);

	return NoError;
}


int4 Motion::run(bool enter)
{
	int4 err = 0;
	if( err ) {
		return err;
	}

	if( enter ) {
		err =  gp_plReg->setEnterRequest();
	}

	return err;
}
