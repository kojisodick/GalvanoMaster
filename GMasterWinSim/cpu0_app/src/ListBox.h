/*
 * ListBox.h
 *
 *  Created on: 2018/11/15
 *      Author: imao_yusuke
 */

#ifndef SRC_LISTBOX_H_
#define SRC_LISTBOX_H_

#include "Common.h"
#include "CommandData.h"

#define MAX_NUM_OF_CMD_IN_LSTBOX    (0x100000)

class ListBox
{
public:
	enum ErrorNumber
	{
		NoError = 0,
		InvalidWriteIndex,
		InvalidReadIndex,
		BufferOverflowed,
	};

public:
	ListBox(){}
	~ListBox(){}

	void clear();

	int4 startWriting(uint4 index=0);
	int4 startReading(uint4 index=0);

	void startRunning(){ m_status = StsRunning; }
	void stopRunning() { m_status = StsReady; }

	bool isRuning(){ return (m_status==StsRunning) ? true : false; }

	int4 push(const CommandData &data);
	int4 pop(CommandData &data);

	uint4 status()     const {return m_status;}
	uint4 readIndex()  const {return m_readIndex;}
	uint4 writeIndex() const {return m_writeIndex;}

private:
	CommandData   m_command[MAX_NUM_OF_CMD_IN_LSTBOX];
	uint4         m_writeIndex;
	uint4         m_readIndex;
	enum Status: uint4
	{
		StsEmpty   = 0x00,
		StsLoading = 0x01,
		StsReady   = 0x04,
		StsRunning = 0x10,
	} m_status;
};

#endif /* SRC_LISTBOX_H_ */
