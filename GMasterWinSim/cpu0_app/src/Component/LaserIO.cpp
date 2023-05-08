/*
 * LaserIO.cpp
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#include <Component/LaserIO.h>

LaserIO::LaserIO()
{
	for( uint4 i=0; i<NumOfLaserIOPort; ++i ) {
		m_param[i].enabled      = false;
		m_param[i].defaultValue = 0;
		m_param[i].outputValue  = 0;

		m_param[i].ramp.enabled  = false;
		m_param[i].ramp.cycle    = 0;
		m_param[i].ramp.delta    = 0;
		m_param[i].ramp.endValue = 0;

		m_param[i].rampEnd.enabled  = false;
		m_param[i].rampEnd.cycle    = 0;
		m_param[i].rampEnd.delta    = 0;
		m_param[i].rampEnd.endValue = 0;
		m_param[i].rampEnd.tlen     = 0;
	}
}

LaserIO::~LaserIO()
{
	// TODO Auto-generated destructor stub
}

Component * LaserIO::enableIO(uint4 out12, uint4 out16, uint4 out4, uint4 da)
{
	m_param[LaserIODout12].enabled = out12 ? true : false;
	m_param[LaserIODout16].enabled = out16 ? true : false;
	m_param[LaserIODout4 ].enabled = out4  ? true : false;
	m_param[LaserIODa1   ].enabled = da    ? true : false;
	m_param[LaserIODa2   ].enabled = da    ? true : false;

	setIOCtrlBlock();

	return this;
}

Component * LaserIO::enableRamp(uint4 out12, uint4 da1, uint4 da2)
{
	m_param[LaserIODout12].ramp.enabled = out12 ? true : false;
	m_param[LaserIODa1   ].ramp.enabled = da1   ? true : false;
	m_param[LaserIODa2   ].ramp.enabled = da2   ? true : false;

	setIOCtrlBlock();
	return this;
}

Component * LaserIO::enableRampEnd(uint4 out12, uint4 da1, uint4 da2)
{
	m_param[LaserIODout12].rampEnd.enabled = out12 ? true : false;
	m_param[LaserIODa1   ].rampEnd.enabled = da1   ? true : false;
	m_param[LaserIODa2   ].rampEnd.enabled = da2   ? true : false;

	setIOCtrlBlock();
	return this;
}


Component * LaserIO::setDefaultValue(uint4 port, uint4 value)
{
	m_param[port].defaultValue = value;

	setIOParamBlock(port);
	return this;
}

Component * LaserIO::setOutputValue(uint4 port, uint4 value)
{
	m_param[port].outputValue = value;

	setIOParamBlock(port);
	return this;
}

Component * LaserIO::setRamp(uint4 port, uint4 cycle, uint4 delta, uint4 endVal)
{
	m_param[port].ramp.cycle    = cycle;
	m_param[port].ramp.delta    = delta;
	m_param[port].ramp.endValue = endVal;

	setRampBlock(port);
	return this;
}

Component * LaserIO::setRampEnd(uint4 port, uint4 cycle, uint4 delta, uint4 endVal, int4 tlen)
{
	m_param[port].rampEnd.cycle    = cycle;
	m_param[port].rampEnd.delta    = delta;
	m_param[port].rampEnd.endValue = endVal;
	m_param[port].rampEnd.tlen     = tlen;

	setRampEndBlock(port);
	return this;
}

void LaserIO::setIOCtrlBlock()
{
	m_ctrlBlock.code      = PlBlock::CodeLaserIoControl;

	m_ctrlBlock.io.dout12 = m_param[LaserIODout12].enabled;
	m_ctrlBlock.io.dout16 = m_param[LaserIODout16].enabled;
	m_ctrlBlock.io.dout4  = m_param[LaserIODout4 ].enabled;
	m_ctrlBlock.io.da1    = m_param[LaserIODa1   ].enabled;
	m_ctrlBlock.io.da1    = m_param[LaserIODa1   ].enabled;

	m_ctrlBlock.ramp.dout12 = m_param[LaserIODout12].ramp.enabled;
	m_ctrlBlock.ramp.da1    = m_param[LaserIODa1   ].ramp.enabled;
	m_ctrlBlock.ramp.da1    = m_param[LaserIODa1   ].ramp.enabled;

	m_ctrlBlock.rampEnd.dout12 = m_param[LaserIODout12].rampEnd.enabled;
	m_ctrlBlock.rampEnd.da1    = m_param[LaserIODa1   ].rampEnd.enabled;
	m_ctrlBlock.rampEnd.da1    = m_param[LaserIODa1   ].rampEnd.enabled;

	m_baseBlock = (PlBlock *)&m_ctrlBlock;
}

void LaserIO::setIOParamBlock(uint4 port)
{
	m_paramBlock.code       = (PlBlock::Code)(PlBlock::CodeLaserDout12Conf+port);
	m_paramBlock.defaultVal = m_param[port].defaultValue;
	m_paramBlock.outputVal  = m_param[port].outputValue;

	if( port == LaserIODout12 || port == LaserIODa1 || port == LaserIODa2 ) {
		m_paramBlock.defaultVal = m_paramBlock.defaultVal << 4;
		m_paramBlock.outputVal  = m_paramBlock.outputVal  << 4;
	}

	m_baseBlock = (PlBlock *)&m_paramBlock;
}

void LaserIO::setRampBlock(uint4 port)
{
	m_rampBlock.code   = (PlBlock::Code)(PlBlock::CodeRampDout12Conf+port);
	m_rampBlock.cycle  = m_param[port].ramp.cycle;
	m_rampBlock.delta  = m_param[port].ramp.delta;
	m_rampBlock.endVal = m_param[port].ramp.endValue;

	m_baseBlock = (PlBlock *)&m_rampBlock;
}

void LaserIO::setRampEndBlock(uint4 port)
{
	m_rampEndBlock.code   = (PlBlock::Code)(PlBlock::CodeRampEndDout12Conf+port);
	m_rampEndBlock.cycle  = m_param[port].rampEnd.cycle;
	m_rampEndBlock.delta  = m_param[port].rampEnd.delta;
	m_rampEndBlock.endVal = m_param[port].rampEnd.endValue;
	m_rampEndBlock.tlen   = m_param[port].rampEnd.tlen;

	m_baseBlock = (PlBlock *)&m_rampEndBlock;
}

