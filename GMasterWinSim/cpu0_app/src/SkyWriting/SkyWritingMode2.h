/*
 * SkyWritingMode2.h
 *
 *  Created on: 2020/03/24
 *      Author: imao_yusuke
 */

#ifndef SRC_SKYWRITING_SKYWRITINGMODE2_H_
#define SRC_SKYWRITING_SKYWRITINGMODE2_H_

#include "SkyWriting/SkyWriting.h"

class SkyWritingMode2: public SkyWriting
{
public:
	SkyWritingMode2();
	~SkyWritingMode2();

	int4 calculate(Motion *cmd) override;
	int4 calculate(Component *cmd)  override;


};

#endif /* SRC_SKYWRITING_SKYWRITINGMODE2_H_ */
