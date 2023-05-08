/*
 * OpticalSystem.h
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#ifndef SRC_COMPONENT_OPTICALSYSTEM_H_
#define SRC_COMPONENT_OPTICALSYSTEM_H_

#include <Component/Component.h>

class OpticalSystem: public Component
{
public:
	OpticalSystem();
	virtual ~OpticalSystem();

	Component * setParameter(real4 geoComp,  real4 interval, real4 D, real4 d);
	Component * setWeight(const real4 *values, real4 geoComp);
	Component * setBitPerMilli(uint4 axis, real4 value);
	Component * setPulsePerBit(uint4 axis, real4 value);
	Component * setPulsePerRad(uint4 axis, real4 value);

	real4 getBitPerMilli(uint4 axis) { return (axis < MAX_NUM_OF_AXIS) ? m_weight[axis].bitPerMilli : (real4)0.0; }
	real4 getPulsePerBit(uint4 axis) { return (axis < MAX_NUM_OF_AXIS) ? m_weight[axis].pulsePerBit : (real4)0.0; }
	real4 getPulsePerRad(uint4 axis) { return (axis < MAX_NUM_OF_AXIS) ? m_weight[axis].pulsePerRad : (real4)0.0; }

	bool  isGeoCompDisabled()  { return m_disableGeoComp;  }
	real4 getLatticeInterval() { return m_latticeInterval; }
	real4 getGeoCompParam1()   { return m_geoCompParam1;   }
	real4 getGeoCompParam2()   { return m_geoCompParam2;   }

private:
	void setParameterBlock();
	void setWeightBlock();


	bool  m_disableGeoComp;
	real4 m_latticeInterval;
	real4 m_geoCompParam1;
	real4 m_geoCompParam2;

	struct Weight
	{
		real4 bitPerMilli;
		real4 pulsePerBit;
		real4 pulsePerRad;
	} m_weight[MAX_NUM_OF_AXIS];

	PlBlockOpticalSystemConf m_paramBlock;
	PlBlockScaleFactorConf   m_weightBlock;
};

#endif /* SRC_COMPONENT_OPTICALSYSTEM_H_ */
