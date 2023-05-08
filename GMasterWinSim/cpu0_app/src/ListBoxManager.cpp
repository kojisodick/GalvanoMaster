/*
 * ListBoxManager.cpp
 *
 *  Created on: 2018/11/15
 *      Author: imao_yusuke
 */

#include <ListBoxManager.h>

ListBoxManager * ListBoxManager::obj()
{
	static ListBoxManager obj;
	return &obj;
}

void ListBoxManager::init()
{
	m_box[ListBox1].clear();
	m_box[ListBox2].clear();

	m_writeCh = m_readCh = ListBox1;

	m_autoChangeEnabled = m_loopEnabled = false;
}

void ListBoxManager::selectRunningList(ListBoxCh ch)
{
	if( ch >= NumOfListBox ) return;

	m_readCh = ch;
}


int4 ListBoxManager::startWriting(ListBoxCh ch)
{
	if( ch >= NumOfListBox ){
		return CommandInvalidArgument;
	}

	m_writeCh = ch;

	return m_box[m_writeCh].startWriting(0);
}

int4 ListBoxManager::startRunning(ListBoxCh ch)
{
	if( ch >= NumOfListBox ) {
		return CommandInvalidArgument;
	}

	m_readCh = ch;

	return m_box[m_readCh].startReading(0);
}



int4 ListBoxManager::startWritingAtPoint(uint4 point)
{
	if( point >= (NumOfListBox*MAX_NUM_OF_CMD_IN_LSTBOX) ) {
		return CommandInvalidArgument;
	}

	m_writeCh   = (ListBoxCh)(point / MAX_NUM_OF_CMD_IN_LSTBOX);
	uint4 index = (uint4    )(point % MAX_NUM_OF_CMD_IN_LSTBOX);

	return m_box[m_writeCh].startWriting(index);
}

int4 ListBoxManager::startRunningAtPoint(uint4 point)
{
	if( point >= (NumOfListBox*MAX_NUM_OF_CMD_IN_LSTBOX) ) {
		return CommandInvalidArgument;
	}

	m_readCh    = (ListBoxCh)(point / MAX_NUM_OF_CMD_IN_LSTBOX);
	uint4 index = (uint4    )(point % MAX_NUM_OF_CMD_IN_LSTBOX);

	return m_box[m_readCh].startReading(index);

}

bool ListBoxManager::isFinished()
{
	m_box[m_readCh].stopRunning();

	if( m_autoChangeEnabled ){
		m_readCh = (m_readCh==ListBox1) ? ListBox2 : ListBox1;

		m_box[m_readCh].startReading();

		m_autoChangeEnabled = false;

		return false;
	}

	if( m_loopEnabled ) {
		m_box[m_readCh].startReading();
		return false;
	}

	return true;
}
