/*
 * SkyWritingMode1.h
 *
 *  Created on: 2020/03/19
 *      Author: imao_yusuke
 */

#ifndef SRC_SKYWRITING_SKYWRITINGMODE1_H_
#define SRC_SKYWRITING_SKYWRITINGMODE1_H_

#include <SkyWriting/SkyWriting.h>

class SkyWritingMode1: public SkyWriting
{
public:
	SkyWritingMode1();
	~SkyWritingMode1();

	int4 calculate(Motion *cmd) override;
	int4 calculate(Component *cmd)  override;


private:
};

#endif /* SRC_SKYWRITING_SKYWRITINGMODE1_H_ */
