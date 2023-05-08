/*
 * Queue.h
 *
 *  Created on: 2020/03/19
 *      Author: imao_yusuke
 */

#ifndef SRC_QUEUE_H_
#define SRC_QUEUE_H_

#include <string.h>
#include <stdio.h>

template <class T, unsigned int N>
class Queue
{
public:
	Queue(): wrIdx(0), rdIdx(0)
	{
		memset(obj, 0, sizeof(obj));
	}

	void init()
	{
		wrIdx = rdIdx = 0;
		memset(obj, 0, sizeof(obj));
	}

	void push(T obj)
	{
		if(wrIdx >= N) {
			return;
		}
		//printf("push [%x] %d\n", (unsigned int)this, wrIdx);

		this->obj[wrIdx++] = obj;
	}

	T pop()
	{

		if(rdIdx >= wrIdx) {
			return NULL;
		}
		//printf("pop [%x] %d\n", (unsigned int)this, rdIdx);

		return this->obj[rdIdx++];
	}

	bool isEmpty()
	{
		return (wrIdx <= rdIdx ) ? true : false;
	}


private:
	unsigned int wrIdx;
	unsigned int rdIdx;
	T obj[N];
};



#endif /* SRC_QUEUE_H_ */
