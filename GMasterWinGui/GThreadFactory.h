#pragma once
class GThreadFactory
{
public:
	static GThreadFactory * obj();

	void create();
private:
	static void entry(void * args);

};

