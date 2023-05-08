/*
 * SkyWritingMode2.cpp
 *
 *  Created on: 2020/03/24
 *      Author: imao_yusuke
 */

#include <SkyWriting/SkyWritingMode2.h>

SkyWritingMode2::SkyWritingMode2()
{

}

SkyWritingMode2::~SkyWritingMode2()
{
}

int4 SkyWritingMode2::calculate(Component *cmd)
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

	m_nowMtn.clear();
	m_preMtn = m_nowMtn;

	return NoError;
}

int4 SkyWritingMode2::calculate(Motion *cmd)
{
	m_mtnBuf.init();
	m_cmpBuf.init();

	if( !m_enabled ) {
		m_mtnBuf.push(cmd);
		return NoError;
	}

	m_nowMtn.set(cmd);

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
