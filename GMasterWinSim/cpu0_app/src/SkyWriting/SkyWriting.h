/*
 * SkyWriting.h
 *
 *  Created on: 2020/07/20
 *      Author: imao_yusuke
 */

#ifndef SRC_SKYWRITING_SKYWRITING_H_
#define SRC_SKYWRITING_SKYWRITING_H_

#include "Queue.h"
#include "Common.h"

#include "Motion/MotionLinear.h"
#include "Motion/MotionArc.h"
#include "Motion/MotionWait.h"

class Component;
class Scanner;

class SkyWriting
{
public:
	SkyWriting();
	virtual ~SkyWriting();

	void setScannerObj(Scanner *obj){ m_scnrPtr = obj; }

	void enable();
	void disable();

	void clear();

	int4 setParameter(uint4 enable, uint4 wait, uint4 pre, uint4 post);

	void getParameter(uint4 &enable, uint4 &wait, uint4 &pre, uint4 &post);



	virtual int4 calculate(Motion    *cmd);
	virtual int4 calculate(Component *cmd);

	bool isEnabled(){ return m_enabled; }
	bool isEmpty();

	int4 run();

protected:
	static bool  m_enabled;
	static uint4 m_waitCount;
	static uint4 m_preCount;
	static uint4 m_postCount;

	struct MotionInfo
	{
		Motion::Type type;
		int4  cmdStartPos[MAX_NUM_OF_AXIS];
		int4  cmdEndPos[MAX_NUM_OF_AXIS];
		real8 cmdLength;
		int4  prePos[MAX_NUM_OF_AXIS];
		int4  postPos[MAX_NUM_OF_AXIS];
		int4  centerPos[MAX_NUM_OF_AXIS];
		real8 preAngle;
		real8 postAngle;

		MotionInfo();
		void clear();
		void set(Motion *cmd);

	private:
		void setPrePostPos(MotionLinear *cmd);
		void setPrePostPos(MotionArc *cmd);
	};

    static MotionInfo m_preMtn, m_nowMtn;

	static Queue<Motion*, 6> m_mtnBuf;
	static Queue<Component*, 1> m_cmpBuf;

	static Scanner * m_scnrPtr;

	MotionLinear m_preJump;
	MotionWait   m_wait;
	MotionLinear m_preLinear;
	MotionArc    m_preArc;
	MotionLinear m_linear;
	MotionArc    m_arc;
	MotionLinear m_postLinear;
	MotionArc    m_postArc;
	MotionLinear m_postJump;
};

#endif /* SRC_SKYWRITING_SKYWRITING_H_ */
