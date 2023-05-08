/*
 * Server.cpp
 *
 *  Created on: 2020/06/25
 *      Author: imao_yusuke
 */

#ifdef __arm__
#include "ServerLwip.h"
typedef ServerLwip ServerPlatform;
#else
#include "ServerWin.h"
typedef ServerWin ServerPlatform ;
#endif

Server * Server::obj()
{
	static ServerPlatform obj;
	return &obj;
}

Server::Server()
{

}

Server::~Server()
{

}