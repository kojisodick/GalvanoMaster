/*
 * SkyWritingMode1.cpp
 *
 *  Created on: 2020/03/19
 *      Author: imao_yusuke
 */

#include <SkyWriting/SkyWritingMode1.h>

SkyWritingMode1::SkyWritingMode1()
	: SkyWriting()
{
	// TODO Auto-generated constructor stub

}

SkyWritingMode1::~SkyWritingMode1()
{
	// TODO Auto-generated destructor stub
}

int4 SkyWritingMode1::calculate(Component *cmd)
{
	m_mtnBuf.init();
	m_cmpBuf.init();

	if( !m_enabled ) {
		m_cmpBuf.push(cmd);
		return NoError;
	}

	if( m_preMtn.type == Motion::MoveMark ) {
		m_mtnBuf.push(m_postLinear.markSw(m_preMtn.postPos[X], m_preMtn.postPos[Y], m_preMtn.postPos[Z]));
		m_mtnBuf.push(m_postJump.jumpSw(m_preMtn.cmdEndPos[X], m_preMtn.cmdEndPos[Y], m_preMtn.cmdEndPos[Z]));
	}
	else if( m_preMtn.type == Motion::MoveArc ) {
		m_mtnBuf.push(m_postArc.moveSw(m_preMtn.postPos, m_preMtn.centerPos, m_preMtn.postAngle));
		m_mtnBuf.push(m_postJump.jumpSw(m_preMtn.cmdEndPos[X], m_preMtn.cmdEndPos[Y], m_preMtn.cmdEndPos[Z]));
	}

	m_cmpBuf.push(cmd);

	m_nowMtn.clear();
	m_preMtn = m_nowMtn;

	return NoError;
}

int4 SkyWritingMode1::calculate(Motion *cmd)
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
		m_mtnBuf.push(m_postJump.jumpSw(m_preMtn.cmdEndPos[X], m_preMtn.cmdEndPos[Y], m_preMtn.cmdEndPos[Z]));
	}
	else if( m_preMtn.type == Motion::MoveArc ) {
		m_mtnBuf.push(m_postArc.moveSw(m_preMtn.postPos, m_preMtn.centerPos, m_preMtn.postAngle));
		m_mtnBuf.push(m_postJump.jumpSw(m_preMtn.cmdEndPos[X], m_preMtn.cmdEndPos[Y], m_preMtn.cmdEndPos[Z]));
	}

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

	m_mtnBuf.push(cmd);

	m_preMtn = m_nowMtn;

	return NoError;
}
