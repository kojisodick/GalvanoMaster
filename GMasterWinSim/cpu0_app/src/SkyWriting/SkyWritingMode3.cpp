/*
 * SkyWritingMode3.cpp
 *
 *  Created on: 2020/03/24
 *      Author: imao_yusuke
 */

#include <SkyWriting/SkyWritingMode3.h>

SkyWritingMode3::SkyWritingMode3()
{
	// TODO Auto-generated constructor stub

}

SkyWritingMode3::~SkyWritingMode3()
{
	// TODO Auto-generated destructor stub
}

void SkyWritingMode3::setLimit(real8 value)
{
	m_limit = value;
}

int4 SkyWritingMode3::calculate(Component *cmd)
{
	m_mtnBuf.init();
	m_cmpBuf.init();

	if( !m_enabled ) {
		m_cmpBuf.push(cmd);
		return NoError;
	}

	if( m_preMtn.type == Motion::MoveMark ) {
		m_mtnBuf.push(m_postLinear.markSw(m_preMtn.postPos[X], m_preMtn.postPos[Y], m_preMtn.postPos[Z]));
	}
	else if( m_preMtn.type == Motion::MoveArc ) {
		m_mtnBuf.push(m_postArc.moveSw(m_preMtn.postPos, m_preMtn.centerPos, m_preMtn.postAngle));
	}

	m_cmpBuf.push(cmd);

	//while (1) {
	//	if (m_cmpBuf.push(cmd) == true) {
	//		break;
	//	}
	//	SLEEP_USEC(100);
	//}

	m_nowMtn.clear();
	m_preMtn = m_nowMtn;

	return NoError;
}

int4 SkyWritingMode3::calculate(Motion *cmd)
{
	m_mtnBuf.init();
	m_cmpBuf.init();

	if( !m_enabled ) {
		m_mtnBuf.push(cmd);
		return NoError;
	}

	m_nowMtn.set(cmd);

	if( isFormedAngleOverLimit() ) {
		m_mtnBuf.push(cmd);
		m_preMtn = m_nowMtn;
		return NoError;
	}

	if( m_preMtn.type == Motion::MoveMark ) {
		m_mtnBuf.push(m_postLinear.markSw(m_preMtn.postPos[X], m_preMtn.postPos[Y], m_preMtn.postPos[Z]));
	}
	else if( m_preMtn.type == Motion::MoveArc ) {
		m_mtnBuf.push(m_postArc.moveSw(m_preMtn.postPos, m_preMtn.centerPos, m_preMtn.postAngle));
	}

	m_preMtn = m_nowMtn;

	if( m_nowMtn.type == Motion::MoveMark ) {
		m_mtnBuf.push(m_preJump.jumpSw(m_nowMtn.prePos[X], m_nowMtn.prePos[Y], m_nowMtn.prePos[Z]));
		if( m_waitCount ) {
			m_mtnBuf.push(m_wait.wait(m_waitCount));
		}
		m_mtnBuf.push(m_preLinear.markSw(m_nowMtn.cmdStartPos[X], m_nowMtn.cmdStartPos[Y], m_nowMtn.cmdStartPos[Z]));
	}
	else if( m_nowMtn.type == Motion::MoveArc ) {
		m_mtnBuf.push(m_preJump.jumpSw(m_nowMtn.prePos[X], m_nowMtn.prePos[Y], m_nowMtn.prePos[Z]));
		if( m_waitCount ) {
			m_mtnBuf.push(m_wait.wait(m_waitCount));
		}
		m_mtnBuf.push(m_postArc.moveSw(m_nowMtn.prePos, m_nowMtn.centerPos, m_nowMtn.preAngle));
	}
	else if( m_nowMtn.type == Motion::MoveJump ) {
		return NoError;
	}
	else if( m_nowMtn.type == Motion::End ) {
		m_mtnBuf.push(m_postJump.jumpSw(m_nowMtn.cmdEndPos[X], m_nowMtn.cmdEndPos[Y], m_nowMtn.cmdEndPos[Z]));
	}

	m_mtnBuf.push(cmd);

	return NoError;
}

bool SkyWritingMode3::isFormedAngleOverLimit()
{
	real8 cosTheta = -1.0;
	if( m_preMtn.type == Motion::MoveMark ) {
		if( m_nowMtn.type == Motion::MoveMark ) {
			real8 va[MAX_NUM_OF_AXIS], vb[MAX_NUM_OF_AXIS];
			for( uint4 i=0; i<MAX_NUM_OF_AXIS; ++i ) {
				va[i] = m_preMtn.cmdEndPos[i]     - m_preMtn.cmdStartPos[i];
				vb[i] = m_nowMtn.cmdEndPos[i] - m_nowMtn.cmdStartPos[i];
			}
			cosTheta = (va[X]*vb[X] + va[Y]*vb[Y] + va[Z]*vb[Z]) / (m_preMtn.cmdLength * m_nowMtn.cmdLength);
		}
		else if( m_nowMtn.type == Motion::MoveArc ) {
			cosTheta = -1.0;
		}
	}
	else if( m_preMtn.type == Motion::MoveArc ) {
		if( m_nowMtn.type == Motion::MoveMark ) {
			cosTheta = -1.0;
		}
		else if( m_nowMtn.type == Motion::MoveArc ) {
			cosTheta = -1.0;
		}
	}

	if( cosTheta >= m_limit ) {
		return true;
	}

	return false;
}

