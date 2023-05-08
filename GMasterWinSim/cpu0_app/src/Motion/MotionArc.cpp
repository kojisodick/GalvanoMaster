/*
 * MotionArc.cpp
 *
 *  Created on: 2020/07/20
 *      Author: imao_yusuke
 */

#include <Motion/MotionArc.h>

MotionArc::MotionArc()
{
	type = MoveArc;
}

MotionArc::~MotionArc()
{
	// TODO Auto-generated destructor stub
}

Motion * MotionArc::moveAbs(int4 cx, int4 cy, real8 angle_degree)
{
	angle_degree = f_limit(angle_degree, 360.0);

	for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
		startPos[i] = endPos[i];
	}

	angle =  (real8)UNITCNVT_DEG_TO_RAD(angle_degree);

	angle = f_limit(angle, M_2PI);


	::rotate2D(endPos[X], endPos[Y],
			startPos[X], startPos[Y],
			this->angle,
			cx, cy);

	radius = ::distance2D(cx, cy, startPos[X], startPos[Y]);

	length = f_abs(radius * angle);


	m_block.mode         = PlBlockArcMove::ModeMark;
	m_block.cmdPos[X]    = endPos[X];
	m_block.cmdPos[Y]    = endPos[Y];
	m_block.centerpos[X] = cx;
	m_block.centerpos[Y] = cy;
	m_block.angle        = angle;

	return this;
}

Motion * MotionArc::moveRel(int4 cx, int4 cy, real8 angle_degree)
{
	return moveAbs(
			cx + endPos[X],
			cy + endPos[Y],
			angle_degree
			);
}

Motion * MotionArc::moveSw(int4 *cmdPos, int4 *centerPos, real8 angle)
{
	m_block.mode  = PlBlockArcMove::ModeSkyWriting;
	m_block.angle = angle;
	memcpy(m_block.cmdPos, cmdPos, sizeof(m_block.cmdPos));
	memcpy(m_block.centerpos, centerPos, sizeof(m_block.centerpos));

	return this;
}

int4 MotionArc::run(bool enter)
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

void MotionArc::convertCoordinate()
{
	rotate2D(m_block.cmdPos[X], m_block.cmdPos[Y],
			m_block.cmdPos[X], m_block.cmdPos[Y],
			m_rotationAngle);

	for( uint4 i=0; i<2; ++i ) {
		m_block.cmdPos[i] *= m_magnification[i];
		m_block.cmdPos[i] += m_offset[i];
		m_block.cmdPos[i]  = (int4)f_limit(m_block.cmdPos[i], 0x7FFFF);

		m_block.centerpos[i] *= m_magnification[i];
		m_block.centerpos[i] += m_offset[i];
		m_block.centerpos[i]  = (int4)f_limit(m_block.cmdPos[i], 0x7FFFF);
	}
}

