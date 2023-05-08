/*
 * ListBoxManager.h
 *
 *  Created on: 2018/11/15
 *      Author: imao_yusuke
 */

#ifndef SRC_LISTBOXMANAGER_H_
#define SRC_LISTBOXMANAGER_H_

#include "ListBox.h"

class Dpram;

class ListBoxManager
{
public:
	static ListBoxManager * obj();

	~ListBoxManager(){}

	void init();
	void selectRunningList(ListBoxCh ch);

	int4 startWriting(ListBoxCh ch);
	int4 startRunning(ListBoxCh ch);
	int4 startWritingAtPoint(uint4 point);
	int4 startRunningAtPoint(uint4 point);

	void startRunning(){ m_box[m_readCh].startRunning(); }
	void stopRunning() { m_box[m_readCh].stopRunning();  }

	int4 addCommand(const CommandData &data) { return m_box[m_writeCh].push(data); }
	int4 getCommand(CommandData &data)       { return m_box[m_readCh].pop(data);   }

	void setAutoChange(bool enabled){ m_autoChangeEnabled = enabled; }
	void setLoop(bool enabled){ m_loopEnabled = enabled; }

	bool isFinished();

	ListBoxCh getWriteCh(){ return m_writeCh; }
	ListBoxCh getReadCh() { return m_readCh; }

	uint4 getStatusVal()  { return m_box[ListBox1].status() | (m_box[ListBox2].status()<<ListBox2) | 0xFF00; }
	uint4 getReadIndex()  { return m_box[m_readCh].readIndex(); }
	uint4 getWriteIndex() { return m_box[m_writeCh].writeIndex(); }

	uint4 getStatusVal(ListBoxCh ch) { return ( ch >= NumOfListBox ) ? 0 :  m_box[ch].status();     }
	uint4 getReadIndex(ListBoxCh ch) { return ( ch >= NumOfListBox ) ? 0 :  m_box[ch].readIndex();  }
	uint4 getWriteIndex(ListBoxCh ch){ return ( ch >= NumOfListBox ) ? 0 :  m_box[ch].writeIndex(); }

	bool isRunning()           { return m_box[m_readCh].isRuning(); }
	bool isAutoChangeEnabled() { return m_autoChangeEnabled; }
	bool isLoopEnabled()       { return m_loopEnabled; }

private:
	ListBoxManager() {}

	ListBox    m_box[NumOfListBox];
	ListBoxCh  m_writeCh;
	ListBoxCh  m_readCh;

	bool       m_autoChangeEnabled;
	bool       m_loopEnabled;
};

#endif /* SRC_LISTBOXMANAGER_H_ */
