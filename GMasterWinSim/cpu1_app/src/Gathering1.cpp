/*
 * Gathering.cpp
 *
 *  Created on: 2018/12/03
 *      Author: imao_yusuke
 */

#include "Gathering1.h"

//#include "xparameters.h"
#include "Dpram.h"

extern Dpram *gp_dpram;

uint4 fix8Data(const void *src)
{
	const fix8 *ptr = (const fix8 *)src;

	real4 data = (real4)ptr->toDouble();
	uint4 *tmp = (uint4 *)&data;

	return *tmp;
};


static const uint4 dummyData = 0xffffffff;



GatItem Gathering1::m_itemList[MaxNumOfGatItems] = {};


Gathering1::Gathering1(): m_state(Stopped)
{
	this->init();

	for(uint4 i=0; i<MaxNumOfGatItems; ++i) {
		m_itemList[i] = { (const void *)&dummyData, dataFormat<uint4> };
	}
}


void Gathering1::init()
{
	if( m_state == Running ) {
		return;
	}

	m_state   = Stopped;
	m_bufSize = MAX_NUM_OF_GAT_DATA;
	m_period  = 0;
	m_bufLoop = false;

	memset(m_buffer, 0, sizeof(m_buffer));

	m_dataCounter   = 0;
	m_periodCounter = 0;

	this->initItems();
}

void Gathering1::initItems()
{
	if( m_state == Running ) {
		return;
	}

	for( uint4 ch=0; ch<MAX_NUM_OF_GAT_CH; ++ch ) {
		m_item[ch] = GatItemBoardStatus1;
	}
}


void Gathering1::registerToItemList(GatItemNo num, const void *srcPtr, format_func format)
{
	if( num > MaxNumOfGatItems ) return;

	m_itemList[num] = { srcPtr , format };
}


int4 Gathering1::selectItem(const uint4 ch, const GatItemNo num)
{
	if( m_state == Running ) {
		return GatErrGatIsRunning;
	}

	if( ch >= MAX_NUM_OF_GAT_CH ) {
		return GatErrInvalidChannel;
	}

	if( num >= MaxNumOfGatItems ) {
		return GatErrInvalidItem;
	}

	m_item[ch] = num;

//	xil_printf("Gathering::%s(%d,%d)\r\n", __FUNCTION__, ch, num);

	return GatErrNoError;
}


int4 Gathering1::setConfig(const uint4 period, const uint4 bufSize, const bool bufLoop)
{
	if( m_state == Running ) {
		return GatErrGatIsRunning;
	}

	m_period  = (period  > MAX_VAL_OF_GAT_PERIOD) ? MAX_VAL_OF_GAT_PERIOD : period;
	m_bufSize = (bufSize > MAX_NUM_OF_GAT_DATA)   ? MAX_NUM_OF_GAT_DATA   : bufSize;
	m_bufLoop = bufLoop;

//	Log::debug("Gathierng::%s(%d,%d,%d)\r\n", __FUNCTION__, m_period, m_bufSize, m_bufLoop);

	return GatErrNoError;
}


int4 Gathering1::start()
{
	if( m_state == Uploading ) {
		return GatErrGatIsUploading;
	}

	m_dataCounter   = 0;
	m_periodCounter = 0;

	m_state = Running;

	return GatErrNoError;
}

void Gathering1::stop()
{
	if( m_state != Running ) {
		return;
	}

	m_state = Stopped;
}

void Gathering1::waitForStart()
{
	if( m_state == Uploading ) {
		return;
	}

	m_dataCounter   = 0;
	m_periodCounter = 0;

}

void Gathering1::waitForEnd()
{
	if( m_state != Running ) {
		return;
	}

}

void Gathering1::measure()
{
	bool signal = (gp_dpram->getGatFlag()==1) ? true : false;

	if( m_state != Running && !signal) {
		return;
	}

	if( m_periodCounter < m_period ) {
		m_periodCounter++;
		return;
	}
	m_periodCounter = 0;

	if( m_dataCounter >= m_bufSize ) {
		if( m_bufLoop ) {
			m_dataCounter = 0;
		}
		else {
			m_state = Stopped;
			return;
		}
	}

	for(uint4 ch=0; ch<MAX_NUM_OF_GAT_CH; ++ch){
		m_buffer[ch][m_dataCounter] = m_itemList[m_item[ch]].format(m_itemList[m_item[ch]].srcPtr);
	}

 	m_dataCounter++;
}

