
#include "MotionManager.h"
#include "Commands.h"
#include "CommandData.h"
#include "Message.h"
#include "Server.h"

MotionManager::MotionManager()
{
	m_hMutex = CreateMutex(NULL, FALSE, NULL);
}

MotionManager::~MotionManager()
{

}

MotionManager * MotionManager::obj()
{
	static MotionManager obj;
	return &obj;
}

void MotionManager::uploadListProgramToFpga()
{

}



void MotionManager::executeCommand(const Message *inMsg, int4 data_offset, Message * resMsg)
{
	uint4 dataLen = inMsg->length - SIZE_OF_MSG_HEADER;

	switch (inMsg->type)
	{
	case MsgTypeCommand:
	{
		uint4 offset = 0;
		uint1 * bar = (uint1 *)&(inMsg->data[data_offset]);

		CommandData command;
		while (offset < dataLen) {
			memcpy((void *)&command,(void *)(bar+offset), sizeof(CommandData));

			resMsg->error = Commands::obj()->run(command);

			memcpy(&(resMsg->data[offset]), (void *)&command, sizeof(CommandData));

			offset += SIZE_OF_CMDDATA;

			if (resMsg->error != Server::NoError) {
				//break;
			}
		}
		resMsg->length += offset;

		break;
	}

	case MsgTypeUpload:
	{
		uint4 offset = inMsg->dataOffset;
		Server * server = Server::obj();

		server->buffer.dataLen = offset + dataLen;

		if (server->buffer.dataLen >= MAX_SIZE_OF_DATABUF) {
			resMsg->length = SIZE_OF_MSG_HEADER;
			resMsg->error = 1; //TODO:
			resMsg->dataOffset = server->buffer.dataLen;
			break;
		}

		memcpy(&server->buffer.data[offset], inMsg->data, dataLen);

		resMsg->length = SIZE_OF_MSG_HEADER;
		resMsg->dataOffset = server->buffer.dataLen;

		break;
	}


	case MsgTypeDownload:
	{
		uint4 offset = inMsg->dataOffset;
		Server * server = Server::obj();

		uint4 remainingDataLen = server->buffer.dataLen - offset;
		if (remainingDataLen >= SIZE_OF_MSG_BODY) {
			memcpy(resMsg->data, &server->buffer.data[offset], SIZE_OF_MSG_BODY);

			resMsg->length += SIZE_OF_MSG_BODY;
			resMsg->dataOffset = offset + SIZE_OF_MSG_BODY;
		}
		else {
			memcpy(resMsg->data, &server->buffer.data[offset], remainingDataLen);

			resMsg->error = 1;
			resMsg->length += remainingDataLen;
			resMsg->dataOffset = server->buffer.dataLen;
		}

		break;
	}

	default:
		break;
	}


}