#pragma once

#include "Server.h"

class ServerWin : public Server
{
private:
	ServerWin();
	friend Server * Server::obj();

public:
	~ServerWin();

	int4 init() override;
	int4 start() override;
	int4 runTcpTask() override;

private:

	bool m_isInit;
	WSADATA      m_wsaData;
	SOCKET       m_socket;
	sockaddr_in  m_server;
	uint4        m_errNum;

	int4 runCommunicationSub(SOCKET &sock);
};