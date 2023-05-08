/*
 * Motion.h
 *
 *  Created on: 2020/03/13
 *      Author: imao_yusuke
 */

#ifndef SRC_MOTION_MOTION_H_
#define SRC_MOTION_MOTION_H_

#include "Common.h"
#include "PL/PlBlockStructure.h"

#include "PL/PlRegister.h"
extern PlRegister *gp_plReg;

#include <Logger/Logger.h>
extern Logger *gp_log;


class Motion
{
public:
	Motion();
	virtual ~Motion(){}

	int4 setOffset(uint4 axis, int4 value);
	int4 setMagnification(uint4 axis, real8 value);
	int4 setRotationAngle(real8 value);

	virtual int4 run(bool enter=false);

//	bool isJump(){ type == MoveJump ? true : false; }
//	bool isMark(){ type == MoveMark ? true : false; }
//	bool isArc() { type == MoveArc  ? true : false; }


	enum Type
	{
		Invalid = 0,
		Begin,
		End,
		MoveJump,
		MoveMark,
		MoveArc,
		MoveWait,
		MoveSkyWrite,
	};

public:
	Type  type;
	static int4 startPos[MAX_NUM_OF_AXIS];
	static int4 endPos[MAX_NUM_OF_AXIS];
	static real8 length;

	static int4  m_offset[MAX_NUM_OF_AXIS];
	static real8 m_magnification[MAX_NUM_OF_AXIS];
	static real8 m_rotationAngle;

	static PlBlock *m_baseBlock;
};

#endif /* SRC_MOTION_MOTION_H_ */
