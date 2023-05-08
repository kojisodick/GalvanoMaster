/*
 * LaserIO.h
 *
 *  Created on: 2020/07/30
 *      Author: imao_yusuke
 */

#ifndef SRC_COMPONENT_LASERIO_H_
#define SRC_COMPONENT_LASERIO_H_

#include <Component/Component.h>

enum LaserIOPort: uint4
{
	LaserIODout12 = 0,
	LaserIODout16,
	LaserIODout4,
	LaserIODa1,
	LaserIODa2,

	NumOfLaserIOPort
};

class LaserIO: public Component
{
public:
	LaserIO();
	virtual ~LaserIO();

	Component * enableIO(uint4 out12, uint4 out16, uint4 out4, uint4 da);
	Component * enableRamp(uint4 out12, uint4 da1, uint4 da2);
	Component * enableRampEnd(uint4 out12, uint4 da1, uint4 da2);

	Component * setDefaultValue(uint4 port, uint4 value);
	Component * setOutputValue(uint4 port, uint4 value);

	Component * setRamp(uint4 port, uint4 cycle, uint4 delta, uint4 endVal);
	Component * setRampEnd(uint4 port, uint4 cycle, uint4 delta, uint4 endVal, int4 tlen);


private:
	void setIOCtrlBlock();
	void setIOParamBlock(uint4 port);
	void setRampBlock(uint4 port);
	void setRampEndBlock(uint4 port);

	struct
	{
		bool  enabled;
		uint4 defaultValue;
		uint4 outputValue;

		struct Ramp
		{
			bool  enabled;
			uint4 cycle;
			uint4 delta;
			uint4 endValue;
		} ramp;

		struct RampEnd
		{
			bool  enabled;
			uint4 cycle;
			uint4 delta;
			uint4 endValue;
			int4  tlen;
		} rampEnd;
	} m_param[NumOfLaserIOPort];

	PlBlockLaserIOControl m_ctrlBlock;
	PlBlockLaserIOParam   m_paramBlock;
	PlBlockRampParam      m_rampBlock;
	PlBlockRampEndParam   m_rampEndBlock;
};

#endif /* SRC_COMPONENT_LASERIO_H_ */
