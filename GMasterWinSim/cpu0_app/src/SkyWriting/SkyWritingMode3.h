/*
 * SkyWritingMode3.h
 *
 *  Created on: 2020/03/24
 *      Author: imao_yusuke
 */

#ifndef SRC_SKYWRITING_SKYWRITINGMODE3_H_
#define SRC_SKYWRITING_SKYWRITINGMODE3_H_

#include <SkyWriting/SkyWriting.h>

class SkyWritingMode3: public SkyWriting
{
public:
	SkyWritingMode3();
	~SkyWritingMode3();

	int4 calculate(Motion *cmd) override;
	int4 calculate(Component *cmd)  override;

	void setLimit(real8 value);

private:
	bool isFormedAngleOverLimit();

	real8 m_limit;
};

#endif /* SRC_SKYWRITING_SKYWRITINGMODE3_H_ */
