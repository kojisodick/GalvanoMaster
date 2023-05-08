/*
 * Laser.h
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#ifndef SRC_COMPONENT_LASER_H_
#define SRC_COMPONENT_LASER_H_

#include <Component/Component.h>

class Laser: public Component
{
public:
	Laser();
	virtual ~Laser();

	Component * setMode(uint4 mode, uint4 gate, uint4 pulse1, uint4 pulse2);
	Component * setTiming(uint4 mode, uint4 halfPeriod, uint4 p1Width, uint4 p2Width);
	Component * setDelay(uint4 onDelay, uint4 offDelay);
	Component * setStandby(uint4 half_period, uint4 pulse_width);

	Component * enable();
	Component * disable();
	Component * signalOn(uint4 count=0);
	Component * signalOff();

private:
	void setConfBlock();
	void setCtrlBlock();

	bool  m_active;
	bool  m_signalOn;
	uint4 m_mode;
	uint1 m_logic[NumOfLaserSignal];
	uint4 m_halfPeriod;
	uint4 m_pulse1Width;
	uint4 m_pulse2Width;
	uint4 m_onDelay;
	uint4 m_offDelay;
	uint4 m_onTime;

	struct Standby
	{
		uint4 halfPeriod;
		uint4 pulseWidth;
	} m_standby;

	PlBlockLaserConf        m_confBlock;
	PlBlockLaserControl     m_ctrlBlock;
	PlBlockLaserStandbyConf m_standbyBlock;
};

#endif /* SRC_COMPONENT_LASER_H_ */
