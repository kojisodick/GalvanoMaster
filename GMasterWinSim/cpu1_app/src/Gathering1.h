/*
 * Gathering.h
 *
 *  Created on: 2018/12/03
 *      Author: imao_yusuke
 */

#ifndef SRC_GATHERING_H_
#define SRC_GATHERING_H_

#include "Common.h"
//#include "xparameters.h"


#define MAX_NUM_OF_GAT_CH     (16)
#define MAX_NUM_OF_GAT_DATA   (0x100000)
#define MAX_VAL_OF_GAT_PERIOD (255)

enum GatheringErrorNum : int4
{
	GatErrNoError = 0,
	GatErrGatIsRunning,
	GatErrGatIsUploading,
	GatErrInvalidChannel,
	GatErrInvalidItem,
};

enum GatDataFormat
{
	GatUnsignedIntData = 0,
	GatSignedIntData,
	GatFloatData,
};


typedef uint4 (*format_func)(const void *src);

typedef struct GatheringItem
{
	const void *srcPtr;
	format_func format;

} GatItem;


template <typename T>
uint4 dataFormat(const void *src)
{
	const uint4 *ptr = (const uint4 *)src;

	return *ptr;
};


class Gathering1
{
public:
	Gathering1();
	~Gathering1(){}

	void init();
	void initItems();

	void registerToItemList(GatItemNo num, const void *srcPtr, format_func format);
	int4 selectItem(uint4 ch, GatItemNo num);
	int4 setConfig(uint4 period, uint4 bufSize, bool bufLoop=false);

	void getSelectedItem(uint4 ch, GatItemNo &num)
	{
		if( ch >= MAX_NUM_OF_GAT_CH ) {
			num = GatItemBoardStatus1;
			return;
		}

		num = m_item[ch];
	}

	void getConfig(uint4 &period, uint4 &bufSize, bool &bufLoop)
	{
		period  = m_period;
		bufSize = m_bufSize;
		bufLoop = m_bufLoop;
	}

	int4 start();
	void stop();
	void waitForStart();
	void waitForEnd();

	void measure();

	const uint1 * getDataAddr(uint4 ch) const
	{
		if( ch >= MAX_NUM_OF_GAT_CH) {
			return NULL;
		}

		return (uint1 *)&m_buffer[ch][0];
	}

	bool bufferIsFull() const
	{
		return (m_dataCounter == m_bufSize);
	}

	bool isRunning() const
	{
		return (m_state == Running);
	}

	uint4 getNumOfData() const
	{
		return m_dataCounter;
	}


private:
	enum State: uint4
	{
		Stopped    = 0,
		Running,
		Uploading,
		WaitForStart,
		WaitForEnd,
	} m_state;

	uint4     m_bufSize; // [1~0x100000]
	uint4     m_period;  // [control cycle]
	bool      m_bufLoop;

	GatItemNo m_item[MAX_NUM_OF_GAT_CH];
	uint4     m_buffer[MAX_NUM_OF_GAT_CH][MAX_NUM_OF_GAT_DATA];

	uint4     m_dataCounter;
	uint4     m_periodCounter;

	static GatItem m_itemList[MaxNumOfGatItems];
};

#endif /* SRC_GATHERING_H_ */
