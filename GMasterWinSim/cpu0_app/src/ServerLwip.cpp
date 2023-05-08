/*
 * ServerLwip.cpp
 *
 *  Created on: 2018/12/05
 *      Author: imao_yusuke
 */

#ifdef __arm__

#include <Server.h>
#include "lwip/init.h"      //lwip_init()
#include "lwip/tcp.h"       //netif
#include "lwip/tcp_impl.h"  //tcp_fasttmr(),tcp_slowtmr()
#include "netif/xadapter.h" //xemac_add()
#include "netif/xemacpsif.h"
#include "xparameters.h"    //XPAR_XEMACPS_0_BASEADDR

#include "Message.h"

#include <Logger/Logger.h>
extern Logger *gp_log;

#include "Commands.h"
extern Commands  *gp_gbCommand;
extern Server    *gp_server;

static netif g_netif;  //network interface

extern volatile uint4 g_tcpFastTimerFlag;
extern volatile uint4 g_tcpSlowTimerFlag;
extern volatile uint4 g_RxDResetFlag;
extern volatile uint4 g_serverWatchDogCntr;


static void print_ip(const char *msg, const ip_addr &ip)
{
	gp_log->info(msg);
	gp_log->info("  %d.%d.%d.%d", ip4_addr1(&ip), ip4_addr2(&ip),
			ip4_addr3(&ip), ip4_addr4(&ip));
}

static void print_ip_settings(const ip_addr &ip, const ip_addr &mask, const ip_addr &gw)
{
	print_ip("Board IP:", ip);
	print_ip("Netmask:", mask);
	print_ip("Gateway:", gw);
}

static int1 accept_callback(void *arg, tcp_pcb *newpcb, int1 err);
static int1 recv_callback(void *arg, tcp_pcb *tpcb, pbuf *p, int1 err);


/*----------------------------------------------------------------------
 * Server Class
 *----------------------------------------------------------------------*/
ServerLwip::ServerLwip()
{
}

ServerLwip::~ServerLwip()
{

}

//----------------------------------------------------------------------
int4 ServerLwip::init()
{
	gp_log->info("[Server::init] Start.");
	lwip_init();

	uint1 boardId = gp_gbCommand->getBoardID();

    ip_addr  m_ipaddr;
	ip_addr  m_netmask;
	ip_addr  m_gateway;

	IP4_ADDR(&m_ipaddr,  192, 168, 200, 10+boardId);
	IP4_ADDR(&m_netmask, 255, 255, 255,  0);
	IP4_ADDR(&m_gateway, 192, 168, 200,  1);

	uint1 mac_ethr_addr[] = {0x00, 0x0a, 0x35, 0x00, 0x01, (uint1)(0x02+boardId)};

	if( !xemac_add(&g_netif,
			&m_ipaddr,
			&m_netmask,
			&m_gateway,
			mac_ethr_addr,
			XPAR_XEMACPS_0_BASEADDR) ) {

		gp_log->error("Error adding N/W interface");

		return InitFailed;
	}

	netif_set_default(&g_netif);

	netif_set_up(&g_netif);


	print_ip_settings(m_ipaddr, m_netmask, m_gateway);
	gp_log->info("MAC Addr:");
	gp_log->info("  %x.%x.%x.%x.%x.%x",
			mac_ethr_addr[0], mac_ethr_addr[1], mac_ethr_addr[2],
			mac_ethr_addr[3], mac_ethr_addr[4], mac_ethr_addr[5]);

	buffer.clear();

	return NoError;
}

//----------------------------------------------------------------------
int4 ServerLwip::start()
{
	uint1 m_portNum = 7;

	tcp_pcb *pcb = tcp_new();

	if ( !pcb ) {
		gp_log->error("[Server::run] PCB creation failed. Out of Memory");
		return PCBCreationFailed;
	}

	err_t err = tcp_bind(pcb, IP_ADDR_ANY, m_portNum);

	if ( err != ERR_OK ) {
		gp_log->error("[Server::run] Unable to bind to port %d: (err=%d)", m_portNum, err);
		return BindFailed;
	}

	tcp_arg(pcb, NULL);

	pcb = tcp_listen(pcb);

	if ( !pcb ) {
		gp_log->error("[Server::run] Out of memory while tcp_listen");
		return ListenFailed;
	}

	tcp_accept(pcb, accept_callback);


	gp_log->info("[Server::run] Command server started @ port %d", m_portNum);

	while( true ) {
		if ( g_tcpFastTimerFlag ) {
			tcp_fasttmr();
			g_tcpFastTimerFlag = 0;
		}
		if ( g_tcpSlowTimerFlag ) {
			tcp_slowtmr();
			g_tcpSlowTimerFlag = 0;
			static uint4 cnt = 0;
			cnt = (cnt == 0xFFFFFFFF) ? 0 : cnt+1;
			//gp_log->info("WatchDogCounter=%u", cnt);
		}

		xemacif_input(&g_netif);

		gp_gbCommand->runListCommands();

//		if( g_RxDResetFlag ) {
//			xemacpsif_resetrx_on_no_rxdata(&g_netif);
//			g_RxDResetFlag = 0;
//		}

//		if( g_serverWatchDogCntr > 120 ) {
//			gp_log->info("PS WatchDog");
//			g_serverWatchDogCntr = 0;
//		}
	}

	return NoError;
}

//----------------------------------------------------------------------
int1 accept_callback(void *arg, tcp_pcb *newpcb, int1 err)
{
	static int4 connection = 0;
	connection++;

	gp_log->info("[Server::accept_callback] Accept connection(%d).",connection);

	tcp_recv(newpcb, recv_callback);

	tcp_arg(newpcb, (void*)(UINTPTR)connection);


	return Server::NoError;
}

//----------------------------------------------------------------------
int1 recv_callback(void *arg, tcp_pcb *tpcb, pbuf *p, int1 err)
{
	static Message recvMsg;
	static uint4   recvMsgOffset = 0;

	g_serverWatchDogCntr = 0;

	if ( !p ) {
		gp_log->info("[Server::recv_callback] Connection was closed.");
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		recvMsg.init();
		recvMsgOffset = 0;
		return Server::NoError;
	}

	tcp_recved(tpcb, p->len);


	if( (p->len + recvMsgOffset) > MAX_SIZE_OF_MSG ) {
		gp_log->info("[Server::recv_callback] Message size is invalid. (len=%d)", p->len);
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		recvMsg.init();
		recvMsgOffset = 0;
		return Server::InvalidRecvMessage;
	}

	uint1 *tempPtr = (uint1*)&recvMsg;
	memcpy(&tempPtr[recvMsgOffset], p->payload, p->len);
	recvMsgOffset += p->len;

	/* free the received pbuf */
	pbuf_free(p);

	if( recvMsg.length > recvMsgOffset ){
		return Server::NoError;
	}
	else if( recvMsg.length < recvMsgOffset ){
		gp_log->error("[Server::recv_callback] Message size is mismatched. (cmd=%d, act=%d)", recvMsg.length, recvMsgOffset);
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		recvMsg.init();
		recvMsgOffset = 0;
		return Server::InvalidRecvMessage;
	}

//	recvMsg.showHeader();

	Message sendMsg;

	uint4 dataLen = recvMsg.length - SIZE_OF_MSG_HEADER;

	switch( recvMsg.type )
	{
	    case MsgTypeCommand:
	    {
	    	uint4 offset  = 0;
			CommandData *ptr;
			while( offset < dataLen ) {
				ptr = (CommandData*)&recvMsg.data[offset];

				sendMsg.error = gp_gbCommand->run(*ptr);

				memcpy(&sendMsg.data[offset], ptr, SIZE_OF_CMDDATA);

				offset += SIZE_OF_CMDDATA;

				if( sendMsg.error != Server::NoError ) {
					//break;
				}
			}
			sendMsg.length += offset;

			break;
		}

	    case MsgTypeUpload:
	    {
	    	uint4 offset = recvMsg.dataOffset;

	    	gp_server->buffer.dataLen = offset + dataLen;

	    	if( gp_server->buffer.dataLen >= MAX_SIZE_OF_DATABUF ) {
		    	sendMsg.length     = SIZE_OF_MSG_HEADER;
	    		sendMsg.error      = 1; //TODO:
		    	sendMsg.dataOffset = gp_server->buffer.dataLen;
	    		break;
	    	}

	    	memcpy(&gp_server->buffer.data[offset], recvMsg.data, dataLen);

	    	sendMsg.length     = SIZE_OF_MSG_HEADER;
	    	sendMsg.dataOffset = gp_server->buffer.dataLen;

	    	break;
	    }


	    case MsgTypeDownload:
	    {
	    	uint4 offset = recvMsg.dataOffset;

    		uint4 remainingDataLen = gp_server->buffer.dataLen - offset;
	    	if( remainingDataLen >= SIZE_OF_MSG_BODY ) {
		    	memcpy(sendMsg.data, &gp_server->buffer.data[offset], SIZE_OF_MSG_BODY);

				sendMsg.length     += SIZE_OF_MSG_BODY;
				sendMsg.dataOffset  = offset + SIZE_OF_MSG_BODY;
	    	}
	    	else {
		    	memcpy(sendMsg.data, &gp_server->buffer.data[offset], remainingDataLen);

		    	sendMsg.error       = 1;
				sendMsg.length     += remainingDataLen;
				sendMsg.dataOffset  = gp_server->buffer.dataLen;
	    	}

	    	break;
	    }

	    default:
	    	break;
	}

	sendMsg.type        = recvMsg.type;
	sendMsg.sequenceNum = recvMsg.sequenceNum;

	if ( tcp_sndbuf(tpcb) > sendMsg.length ) {
		err = tcp_write(tpcb, &sendMsg, sendMsg.length, TCP_WRITE_FLAG_COPY);
	}
	else {
		gp_log->error("[Server::recv_callback] No space in tcp_sndbuf. (len=%d)", sendMsg.length);
		sendMsg.showHeader();

		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		recvMsg.init();
		recvMsgOffset = 0;
	}


	recvMsg.init();
	recvMsgOffset = 0;

	return Server::NoError;
}


#endif
