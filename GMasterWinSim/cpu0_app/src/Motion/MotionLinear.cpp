/*
 * MotionLinear.cpp
 *
 *  Created on: 2020/07/20
 *      Author: imao_yusuke
 */

#include <Motion/MotionLinear.h>

MotionLinear::MotionLinear()
{
	type = MoveJump;

}

MotionLinear::~MotionLinear()
{
	// TODO Auto-generated destructor stub
}

Motion * MotionLinear::jumpAbs(int4 x, int4 y, real8 count)
{
	return this->jumpAbs(x, y, endPos[Z], count);
}

Motion * MotionLinear::jumpAbs(int4 x, int4 y, int4 z, real8 count)
{
	type = MoveJump;

	for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
		startPos[i] = endPos[i];
	}

	endPos[X]    = x;
	endPos[Y]    = y;
	endPos[Z]    = z;
	length       = ::distance3D(startPos[X], startPos[Y], startPos[Z], endPos[X], endPos[Y], endPos[Z]);
	arrivalCount = count;

	m_block.mode         = PlBlockLinearMove::ModeJump;
	m_block.arrivalCount = arrivalCount;
	memcpy(m_block.cmdPos, endPos, sizeof(m_block.cmdPos));

	convertCoordinate();

	return this;
}

Motion * MotionLinear::jumpRel(int4 x, int4 y, real8 count)
{
	return this->jumpAbs(
			x + endPos[X],
			y + endPos[Y],
			count);
}

Motion * MotionLinear::jumpRel(int4 x, int4 y, int4 z, real8 count)
{
	return this->jumpAbs(
			x + endPos[X],
			y + endPos[Y],
			z + endPos[Z],
			count);
}


Motion * MotionLinear::markAbs(int4 x, int4 y, real8 count)
{
	return this->markAbs(x, y, endPos[Z], count);
}

Motion * MotionLinear::markAbs(int4 x, int4 y, int4 z, real8 count)
{
	this->jumpAbs(x, y, z, count);

	type = MoveMark;

	m_block.mode = PlBlockLinearMove::ModeMark;

	return this;
}

Motion * MotionLinear::markRel(int4 x, int4 y, real8 count)
{
	return this->markAbs(
			x + endPos[X],
			y + endPos[Y],
			count);
}

Motion * MotionLinear::markRel(int4 x, int4 y, int4 z, real8 count)
{
	return this->markAbs(
			x + endPos[X],
			y + endPos[Y],
			z + endPos[Z],
			count);
}

Motion * MotionLinear::jumpSw(int4 x, int4 y, int4 z)
{
	m_block.mode         = PlBlockLinearMove::ModeJump;
	m_block.cmdPos[X]    = x;
	m_block.cmdPos[Y]    = y;
	m_block.cmdPos[Z]    = z;
	m_block.arrivalCount = arrivalCount;

	convertCoordinate();

	return this;
}
Motion * MotionLinear::markSw(int4 x, int4 y, int4 z)
{
	m_block.mode         = PlBlockLinearMove::ModeSkyWriting;
	m_block.cmdPos[X]    = x;
	m_block.cmdPos[Y]    = y;
	m_block.cmdPos[Z]    = z;
	m_block.arrivalCount = arrivalCount;

	convertCoordinate();

	return this;
}

int4 MotionLinear::run(bool enter)
{
	int4 err = gp_plReg->setBlock((PlBlock *)&m_block);

	if( err ) {
		return err;
	}

	if( enter ) {
		return gp_plReg->setEnterRequest();
	}

	return NoError;
}

void MotionLinear::convertCoordinate()
{
//	for(int4 i=0; i<MAX_NUM_OF_AXIS; ++i) {
//		gp_log->debug("m_gmagnification[%d]=%f", i , m_magnification[i]);
//		gp_log->debug("m_offset[%d]        =%d", i , m_offset[i]);
//	}
//	gp_log->debug("m_rotationAngle     =%f", m_rotationAngle);
//
//	gp_log->debug("(%d, %d)",	m_block.cmdPos[X], m_block.cmdPos[Y]);

	rotate2D(m_block.cmdPos[X], m_block.cmdPos[Y],
			m_block.cmdPos[X], m_block.cmdPos[Y],
			m_rotationAngle);

	for( uint4 i=0; i<MAX_NUM_OF_AXIS; ++i ) {
		m_block.cmdPos[i] *= m_magnification[i];
		m_block.cmdPos[i] += m_offset[i];
		m_block.cmdPos[i] = (int4)f_limit(m_block.cmdPos[i], 0x7FFFF);
	}

//	gp_log->debug(" -> (%d, %d)",	m_block.cmdPos[X], m_block.cmdPos[Y]);
}

