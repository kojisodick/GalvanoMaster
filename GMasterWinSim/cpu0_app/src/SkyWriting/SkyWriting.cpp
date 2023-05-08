/*
 * SkyWriting.cpp
 *
 *  Created on: 2020/07/20
 *      Author: imao_yusuke
 */

#include <SkyWriting/SkyWriting.h>
#include <math.h>

//#include "Component/Component.h"
#include "Component/Scanner.h"

bool  SkyWriting::m_enabled   = false;
uint4 SkyWriting::m_waitCount = 0;
uint4 SkyWriting::m_preCount  = 0;
uint4 SkyWriting::m_postCount = 0;
Scanner * SkyWriting::m_scnrPtr = NULL;

SkyWriting::MotionInfo SkyWriting::m_preMtn;
SkyWriting::MotionInfo SkyWriting::m_nowMtn;

Queue<Motion*, 6> SkyWriting::m_mtnBuf;
Queue<Component*, 1> SkyWriting::m_cmpBuf;

SkyWriting::SkyWriting()
{
	// TODO Auto-generated constructor stub

}

SkyWriting::~SkyWriting()
{
	// TODO Auto-generated destructor stub
}

void SkyWriting::clear()
{
	m_mtnBuf.init();
	m_cmpBuf.init();
	m_nowMtn.clear();
	m_preMtn.clear();
}

void SkyWriting::enable()
{
	m_enabled = true;
}

void SkyWriting::disable()
{
	if( !m_enabled ) {
		return;
	}

	m_mtnBuf.init();
	m_cmpBuf.init();

	if( m_preMtn.type == Motion::MoveMark ) {
		m_mtnBuf.push(m_postLinear.markSw(m_preMtn.postPos[X], m_preMtn.postPos[Y], m_preMtn.postPos[Z]));
		m_mtnBuf.push(m_postJump.jumpSw(m_preMtn.cmdEndPos[X], m_preMtn.cmdEndPos[Y], m_preMtn.cmdEndPos[Z]));
	}
	else if( m_preMtn.type == Motion::MoveArc ) {
		m_mtnBuf.push(m_postArc.moveSw(m_preMtn.postPos, m_preMtn.centerPos, m_preMtn.postAngle));
		m_mtnBuf.push(m_postJump.jumpSw(m_preMtn.cmdEndPos[X], m_preMtn.cmdEndPos[Y], m_preMtn.cmdEndPos[Z]));
	}

	m_nowMtn.clear();
	m_preMtn = m_nowMtn;

	m_enabled = false;
}


int4 SkyWriting::setParameter(uint4 enable, uint4 wait, uint4 pre, uint4 post)
{
	if( enable == 0 ) {
		this->disable();
	}
	else {
		this->enable();
	}
	m_waitCount = wait;
	m_preCount  = pre;
	m_postCount = post;

	m_preMtn = m_nowMtn;

	return NoError;
}

void SkyWriting::getParameter(uint4 &enable, uint4 &wait, uint4 &pre, uint4 &post)
{
	enable = m_enabled;
	wait   = m_waitCount;
	pre    = m_preCount;
	post   = m_postCount;
}

int4 SkyWriting::calculate(Motion *cmd)
{
	m_mtnBuf.init();
	m_cmpBuf.init();

	m_mtnBuf.push(cmd);

	return NoError;
}

int4 SkyWriting::calculate(Component *cmd)
{
	m_mtnBuf.init();
	m_cmpBuf.init();

	m_cmpBuf.push(cmd);

	return NoError;
}


bool SkyWriting::isEmpty()
{
	return ( m_mtnBuf.isEmpty() && m_cmpBuf.isEmpty() ) ? true : false;
}

int4 SkyWriting::run()
{

	// Only one of mtnBuf or cmpBuf should have a data
	if( !m_mtnBuf.isEmpty() ) {
        Motion * mtn = m_mtnBuf.pop();
        if( mtn == NULL ) {
            gp_log->error("mtn is null");
			return -1;
            //return NoError;
        }
		return mtn->run();
	}

	if( !m_cmpBuf.isEmpty() ) {
        Component * cmp = m_cmpBuf.pop();
        if( cmp == NULL ) {
            gp_log->error("cmp is null");
			return -1;
			//return NoError;
        }
		return cmp->run();
	}

	return NoError; //FIXME
}


SkyWriting::MotionInfo::MotionInfo()
{
	this->clear();
}

void SkyWriting::MotionInfo::clear()
{
	type      = Motion::Invalid;
	cmdLength = 0.0;
	preAngle  = 0.0;
	postAngle = 0.0;

	for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
		cmdStartPos[i] = prePos[i] = postPos[i] = cmdEndPos[i];
		centerPos[i] = 0;
	}
}

void SkyWriting::MotionInfo::set(Motion *cmd)
{
	this->clear();

	type = cmd->type;

	if( cmd->type == Motion::MoveMark ) {
		MotionLinear *ptr = (MotionLinear *)cmd;
		for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
			cmdStartPos[i] = prePos[i]  = ptr->startPos[i];
			cmdEndPos[i]   = postPos[i] = ptr->endPos[i];
		}
		cmdLength = ptr->length;

		if( cmdLength > 0.0 ) {
			setPrePostPos(ptr);
		}

	}
	else if( cmd->type == Motion::MoveArc ) {
		MotionArc *ptr = (MotionArc *)cmd;
		for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
			cmdStartPos[i] = prePos[i]  = ptr->startPos[i];
			cmdEndPos[i]   = postPos[i] = ptr->endPos[i];
			centerPos[i]   = ptr->centerPos[i];
		}
		cmdLength = ptr->length;

		if( cmdLength > 0.0 ) {
			setPrePostPos(ptr);
		}
	}
	else if (cmd->type == Motion::MoveJump ) {
		MotionLinear *ptr = (MotionLinear *)cmd;
		for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
			cmdStartPos[i] = prePos[i]  = ptr->startPos[i];
			cmdEndPos[i]   = postPos[i] = ptr->endPos[i];
		}
		cmdLength = ptr->length;
	}
}

void SkyWriting::MotionInfo::setPrePostPos(MotionLinear *cmd)
{
	real8 m = m_preCount * m_scnrPtr->getMarkSpeed();
	real8 n = cmdLength + m;

	for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
//		prePos[i] = (int4)round((-n * cmd->startPos[i] + m * cmd->endPos[i]) / -(cmdLength));
		prePos[i] = (int4)round((-n * cmdStartPos[i] + m * cmdEndPos[i]) / -(cmdLength));
	}

	n = m_postCount * m_scnrPtr->getMarkSpeed();
	m = cmdLength + n;

	for( int i=0; i<MAX_NUM_OF_AXIS; ++i ) {
		postPos[i] = (int4)round((-n * cmdStartPos[i] + m * cmdEndPos[i]) / (cmdLength));
	}
}

void SkyWriting::MotionInfo::setPrePostPos(MotionArc *cmd)
{
	preAngle = m_preCount * m_scnrPtr->getMarkSpeed() / cmd->radius;
	preAngle = ( preAngle > M_2PI ) ?  M_2PI : preAngle;
	preAngle *= ( cmd->angle > 0.0 ) ?  1.0 : -1.0;

	::rotate2D(prePos[X], prePos[Y],
			cmdStartPos[X], cmdStartPos[Y],
			(-preAngle),  //Rotate in reverse
			centerPos[X], centerPos[Y]);

	postAngle = m_postCount * m_scnrPtr->getMarkSpeed() / cmd->radius;
	postAngle = ( postAngle > M_2PI ) ?  M_2PI : postAngle;
	postAngle *= ( cmd->angle > 0.0 ) ?  1.0 : -1.0;

	::rotate2D(postPos[X], postPos[Y],
			cmdEndPos[X], cmdEndPos[Y],
			postAngle,  //Rotate in reverse
			centerPos[X], centerPos[Y]);
}

