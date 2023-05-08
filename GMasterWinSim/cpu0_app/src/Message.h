/*
 * Message.h
 *
 *  Created on: 2018/11/14
 *      Author: imao_yusuke
 */

#ifndef SRC_SERVER_MESSAGE_H_
#define SRC_SERVER_MESSAGE_H_

#include <Logger/Logger.h>
#include "Common.h"

extern Logger *gp_log;

#define SIZE_OF_MSG_HEADER    (20)
#define SIZE_OF_MSG_BODY      (1400)

#define MAX_SIZE_OF_MSG       (SIZE_OF_MSG_HEADER + SIZE_OF_MSG_BODY)

enum MessageType : uint4
{
	MsgTypeCommand = 0,
	MsgTypeUpload,
	MsgTypeDownload,
};

struct Message
{
	// Header
	uint4 length;
	uint4 sequenceNum;
	uint4 type;
	uint4 error;
	uint4 dataOffset;

	// Body
	uint1 data[SIZE_OF_MSG_BODY];

	Message(){ this->init(); }

	void init()
	{
		length       = SIZE_OF_MSG_HEADER;
		sequenceNum  = 0;
		type         = 0;
		error        = 0;
		dataOffset   = 0;
		memset(data, 0, SIZE_OF_MSG_BODY);
	}

	void setData(const void *payload, const uint4 len)
	{
		gp_log->debug("setData start.");

		uint4 *ptr  = (uint4*)payload;
		length      = ptr[0];
		sequenceNum = ptr[1];
		type        = ptr[2];
		error       = ptr[3];
		dataOffset  = ptr[4];

		uint4 dataLen = len -SIZE_OF_MSG_HEADER;
		if( dataLen > 0 ){
			memcpy(data, &ptr[5], dataLen);
		}
	}

	void showHeader()
	{
#if 0
		gp_log->debug("Message: length=%d sequenceNum=%d type=%d error=%d dataOffset=%d"
				, length, sequenceNum, type, error, dataOffset);
#endif
	}

	void showBody()
	{
#if 0
		gp_log->debug("Message: ");
		uint4 *ptr = (uint4 *)data;
		for(int i=0; i<8; ++i){
			PRINTF("%x ", ptr[i]);
		}
#endif
	}
};

#endif /* SRC_SERVER_MESSAGE_H_ */
