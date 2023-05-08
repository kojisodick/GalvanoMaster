/*
 * PlatformWin.h
 *
 *  Created on: 2018/12/04
 *      Author: imao_yusuke
 */

#ifndef SRC_SYSTEM_PLATFORMWIN_H_
#define SRC_SYSTEM_PLATFORMWIN_H_

#ifndef __arm__

#include "Platform1.h"

class Platform1Win : public Platform1 {
public:
	Platform1Win();
	~Platform1Win();

	int4 init(uint4 controlCycle) override;
	void cleanup() override;

};

#endif

#endif /* SRC_SYSTEM_PLATFORMWIN_H_ */
