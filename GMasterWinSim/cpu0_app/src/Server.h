/*
 * Server.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_SERVER_SERVER_H_
#define SRC_SERVER_SERVER_H_

#include "Common.h"

#define MAX_SIZE_OF_DATABUF   (0x400000) //4Mbyte

class Server
{
public:
	enum ErrorCode
	{
		NoError = 0,
		InitFailed,
		PCBCreationFailed,
		BindFailed,
		ListenFailed,
		InvalidRecvMessage,
	};

public:
	static Server * obj();
	~Server();

	virtual int4 init() = 0;
	virtual int4 start() = 0;
#ifndef __arm__
	virtual int4 runTcpTask() = 0;
#endif

	struct DataBuffer
	{
		uint1 data[MAX_SIZE_OF_DATABUF];
		uint4 dataLen;

		void clear(){
			memset(data, 0, sizeof(data));
			dataLen = 0;
		}

	} buffer;

protected:
	Server();

};

#endif /* SRC_SERVER_SERVER_H_ */
