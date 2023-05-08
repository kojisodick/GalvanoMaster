/*
 * Laser.cpp
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#include <Component/Laser.h>

Laser::Laser()
{
	m_active                   = false;
	m_signalOn                 = false;
	m_mode                     = LaserModeCO2;
	m_logic[LaserSignalGate]   = ActiveHigh;
	m_logic[LaserSignalPulse1] = ActiveHigh;
	m_logic[LaserSignalPulse2] = ActiveHigh;
	m_halfPeriod               = 0xffffff;
	m_pulse1Width              = 2;
	m_pulse2Width              = 3;
	m_onDelay                  = 0;
	m_offDelay                 = 0;
	m_onTime                   = 0;

	m_standby.halfPeriod       = 0xffffff;
	m_standby.pulseWidth       = 2;

}

Laser::~Laser()
{
	// TODO Auto-generated destructor stub
}

Component * Laser::setMode(uint4 mode, uint4 gate, uint4 pulse1, uint4 pulse2)
{
	m_mode = mode;
	m_logic[LaserSignalGate]   = (uint1)gate;
	m_logic[LaserSignalPulse1] = (uint1)pulse1;
	m_logic[LaserSignalPulse2] = (uint1)pulse2;

	setConfBlock();

	return this;
}

Component * Laser::setTiming(uint4 mode, uint4 halfPeriod, uint4 p1Width, uint4 p2Width)
{
	m_mode        = mode;
	m_halfPeriod  = halfPeriod;
	m_pulse1Width = p1Width;
	m_pulse2Width = p2Width;

	setConfBlock();

	return this;
}

Component * Laser::setDelay(uint4 onDelay, uint4 offDelay)
{
	m_onDelay  = onDelay;
	m_offDelay = offDelay;

	setConfBlock();

	return this;
}

Component * Laser::setStandby(uint4 half_period, uint4 pulse_width)
{
	m_standby.halfPeriod = half_period;
	m_standby.pulseWidth = pulse_width;

	m_standbyBlock.halfPeriod = half_period;
	m_standbyBlock.halfPeriod = pulse_width;

	m_baseBlock = (PlBlock *)&m_standbyBlock;

	return this;
}




Component * Laser::enable()
{
	m_active   = true;
	m_signalOn = false;
	m_onTime   = 0;

	setCtrlBlock();

	return this;
}

Component * Laser::disable()
{
	m_active   = false;
	m_signalOn = false;
	m_onTime   = 0;

	setCtrlBlock();

	return this;
}

Component * Laser::signalOn(uint4 count)
{
	if( m_active ) {
		m_signalOn = true;
		m_onTime   = count;
	}
	else {
		m_signalOn = false;
		m_onTime   = 0;
	}

	setCtrlBlock();

	return this;
}

Component * Laser::signalOff()
{
	m_signalOn = false;
	m_onTime    = 0;

	setCtrlBlock();

	return this;
}

void Laser::setConfBlock()
{
	m_confBlock.code         = PlBlock::CodeActLaserConf;
	m_confBlock.mode         = m_mode;
	m_confBlock.logic.gate   = m_logic[LaserSignalGate];
	m_confBlock.logic.pulse1 = m_logic[LaserSignalPulse1];
	m_confBlock.logic.pulse2 = m_logic[LaserSignalPulse2];
	m_confBlock.halfPeriod   = m_halfPeriod;
	m_confBlock.pulse1Width  = m_pulse1Width;
	m_confBlock.pulse2Width  = m_pulse2Width;
	m_confBlock.onDelay      = m_onDelay;
	m_confBlock.offDelay     = m_offDelay;

	m_baseBlock = (PlBlock *)&m_confBlock;
}

void Laser::setCtrlBlock()
{
	m_ctrlBlock.code     = PlBlock::CodeLaserControl;
	m_ctrlBlock.active   = m_active;
	m_ctrlBlock.signalOn = m_signalOn;
	m_ctrlBlock.count    = m_onTime;

	m_baseBlock = (PlBlock *)&m_ctrlBlock;
}

