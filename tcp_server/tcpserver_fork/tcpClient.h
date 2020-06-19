/**
 * tcpClient.h
 *
 * this is a tcp client built with c++
 *
 * @author zhyjc6
 * @date 2020-3-10
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>


#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__
#define MESSAGE_LEN 1024

namespace tcpClient {

class TcpClient {
public:
	TcpClient();
	~TcpClient();

private:
	int conSocket;
	struct sockaddr_in server_addr;
	char sendbuf[MESSAGE_LEN];
	char recvbuf[MESSAGE_LEN];
	
public:
	void toConnect(int port);
	void run();
};

}//namespace tcpClient

#endif//__TCPCLIENT_H__
