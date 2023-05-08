/*
* ServerWinSock.cpp
*
*  Created on: 2018/12/05
*      Author: imao_yusuke
*/

#ifdef WIN_MASTER_SIMULATOR
extern void finish_simulation();
#endif

#ifndef __arm__

#include "ServerWin.h"
#include <winsock.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Message.h"
#include "CommandData.h"
#include "MotionManager.h"
#include "Commands.h"

ServerWin::ServerWin()
	: Server()
{
	m_isInit = false;
}

ServerWin::~ServerWin()
{
	WSACleanup();
}

int4 ServerWin::init()
{
	m_socket = INVALID_SOCKET;
	m_isInit = false;
	return NoError;
}


int4 ServerWin::start()
{

	m_errNum = WSAStartup(MAKEWORD(2, 2), (LPWSADATA)&m_wsaData);

	if (m_errNum != 0) {
		return -1;
	}

	m_isInit = true;
	while (true) {

		int error = Commands::obj()->runListCommands();
	}

	return NoError;
}


int4 ServerWin::runTcpTask()
{
	// wait until the socket is initialized
	while (m_isInit == false ){
		SLEEP_USEC(1000);	
	}

	m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (m_socket == INVALID_SOCKET) {
		printf("socket : %d\n", m_errNum = WSAGetLastError());
		return -1;
	}

	m_server.sin_family = AF_INET;
	m_server.sin_port = htons(7);
	m_server.sin_addr.S_un.S_addr = INADDR_ANY;

	m_errNum = bind(m_socket, (sockaddr *)&m_server, sizeof(m_server));

	if (m_errNum != 0) {
		printf("bind : %d\n", m_errNum = WSAGetLastError());
		return -1;
	}

	m_errNum = listen(m_socket, 5);

	if (m_errNum != 0) {
		printf("listen : %d\n", m_errNum = WSAGetLastError());
		return -1;
	}


	sockaddr_in client;

	while (1) {
		int addrlen = sizeof(sockaddr_in);
		SOCKET sock = accept(m_socket, (struct sockaddr *)&client, &addrlen);

		if (sock == INVALID_SOCKET) {
			printf("error accept : %d\n", m_errNum = WSAGetLastError());
			return -1;
		}
		if (runCommunicationSub(sock) != NoError) {
			closesocket(sock);
			return -1;
		}

	}

	return NoError;

}

int4 ServerWin::runCommunicationSub(SOCKET &sock)
{
	Message      recvMsg;
	Message      sendMsg;
	uint4        cmdDataOffset;
	uint4        cmdDataLen;

	int len;
	uint4 nonblocking;

    while(1) {
        recvMsg.init();
        sendMsg.init();

        nonblocking = 1;
        ioctlsocket(sock, FIONBIO, &nonblocking);

        len = recv(sock, (char*)&recvMsg, MAX_SIZE_OF_MSG, 0);

        if ( len < 0 ) {
            m_errNum = WSAGetLastError();
            if( m_errNum == WSAEWOULDBLOCK ) {
                goto SKIP;
            }
            else {
                printf("recv : %d\n", m_errNum);
                closesocket(sock);
                break;
            }
        }
        else if ( len == 0 ) {
            closesocket(sock);
            break;
        }

		//PRINTF("[Log] Recv : ");
        //recvMsg.showHeader();

        nonblocking = 0;
        ioctlsocket(sock, FIONBIO, &nonblocking);


        cmdDataOffset = 0;
        cmdDataLen = recvMsg.length - SIZE_OF_MSG_HEADER;

        MotionManager::obj()->executeCommand(&recvMsg, cmdDataOffset, &sendMsg);

        sendMsg.sequenceNum = recvMsg.sequenceNum;
        sendMsg.length += cmdDataOffset;

        len = send(sock, (char*)&sendMsg, sendMsg.length, 0);
        if (len < 1) {
            printf("send : %d\n", m_errNum = WSAGetLastError());
            closesocket(sock);
            return -1;
        }

		//PRINTF("[Log] Send : ");
        //sendMsg.showHeader();

SKIP:
		MotionManager::obj()->uploadListProgramToFpga();
    }


    return NoError;
}



#endif //__arm__
