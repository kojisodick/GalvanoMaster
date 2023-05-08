#pragma once

#include <Windows.h>
#include "Common.h"

struct Message;
struct CommandData;

class MotionManager
{
public:
	MotionManager();
	~MotionManager();

	static MotionManager * obj();
	void uploadListProgramToFpga();
	void executeCommand(const Message *inMsg, int4 data_offset, Message * resMsg);

	HANDLE mutex() { return m_hMutex; }
private:
	HANDLE m_hMutex;
};