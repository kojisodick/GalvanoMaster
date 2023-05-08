#pragma once

#include "Platform0.h"

class Platform0Win : public Platform0
{
	friend Platform0 * Platform0::obj();
public:
	int4 init() override;
	void cleanup() override;
	void enableInterrupt() override;
	void disableInterrupt() override;

	~Platform0Win();
private:
	Platform0Win();

	static unsigned int DummyThread0(void *param);

	HANDLE m_hThread;
	uint4  m_threadId;
};