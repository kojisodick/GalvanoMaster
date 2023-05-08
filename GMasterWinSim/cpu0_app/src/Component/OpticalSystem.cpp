/*
 * OpticalSystem.cpp
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#include <Component/OpticalSystem.h>

OpticalSystem::OpticalSystem()
{
	m_disableGeoComp  = false;
	m_latticeInterval = 8192;
	m_geoCompParam1   = (real4)942.3281;
	m_geoCompParam2   = (real4)27.4597;

	m_weight[X].bitPerMilli = (real4)3488;
	m_weight[X].pulsePerBit = (real4) 1.375003;
	m_weight[X].pulsePerRad = (real4)1955696;

	m_weight[Y].bitPerMilli = (real4)3488;
	m_weight[Y].pulsePerBit = (real4)1.375003;
	m_weight[Y].pulsePerRad = (real4)1955696;

	m_weight[Z].bitPerMilli = (real4)104857.6;
	m_weight[Z].pulsePerBit = (real4)1.0;
	m_weight[Z].pulsePerRad = (real4)1955696;
}

OpticalSystem::~OpticalSystem()
{
	// TODO Auto-generated destructor stub
}

Component * OpticalSystem::setParameter(real4 geoComp, real4 interval, real4 D, real4 d)
{
	m_disableGeoComp  = geoComp ? true : false;
	m_latticeInterval = interval;
	m_geoCompParam1   = (real4)(D * 0.001); // [mm] -> [m]
	m_geoCompParam2   = (real4)(d * 0.001); // [mm] -> [m]

	setParameterBlock();

	return this;
}

Component * OpticalSystem::setWeight(const real4 *values, real4 geoComp)
{
	for( int4 i=0; i<MAX_NUM_OF_AXIS; ++i ) {
		m_weight[i].bitPerMilli = values[i];
		m_weight[i].pulsePerBit = values[i+3];
		m_weight[i].pulsePerRad = values[i+6];

		gp_log->debug(" -> set BitPerMilli[%d]=%f", i, values[i]);
		gp_log->debug(" -> set PulsePerBit[%d]=%f", i, values[i+3]);
		gp_log->debug(" -> set PulsePerRad[%d]=%f", i, values[i+6]);
	}

	m_disableGeoComp = geoComp ? true : false;

	setWeightBlock();

	return this;
}

Component * OpticalSystem::setBitPerMilli(uint4 axis, real4 value)
{
	if( axis >= MAX_NUM_OF_AXIS ) {
		return this;
	}

	m_weight[axis].bitPerMilli = value;

	setWeightBlock();
	return this;
}

Component * OpticalSystem::setPulsePerBit(uint4 axis, real4 value)
{
	if( axis >= MAX_NUM_OF_AXIS ) {
		return this;
	}

	m_weight[axis].pulsePerBit = value;

	setWeightBlock();
	return this;
}

Component * OpticalSystem::setPulsePerRad(uint4 axis, real4 value)
{
	if( axis >= MAX_NUM_OF_AXIS ) {
		return this;
	}

	m_weight[axis].pulsePerRad = value;

	setWeightBlock();
	return this;
}

void OpticalSystem::setParameterBlock()
{
	m_paramBlock.param1          = m_geoCompParam1;
	m_paramBlock.param2          = m_geoCompParam2;
	m_paramBlock.latticeInterval = (uint4)m_latticeInterval;
	m_paramBlock.disabledGeoComp = (uint4)m_disableGeoComp;

	m_baseBlock = (PlBlock *)&m_paramBlock;
}

void OpticalSystem::setWeightBlock()
{
	if( m_disableGeoComp ) {
		for( uint4 i=0; i<MAX_NUM_OF_AXIS; ++ i ) {
			m_weightBlock.value[i] = m_weight[i].pulsePerBit;
		}
	}
	else {
		for( uint4 i=0; i<MAX_NUM_OF_AXIS; ++ i ) {
			m_weightBlock.value[i] = 1.0 / m_weight[i].bitPerMilli;
		}
	}

	m_baseBlock = (PlBlock *)&m_weightBlock;
}
