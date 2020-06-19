/**
 * tcpClient.h
 *
 * this is a tcp client
 *
 * @author zhyjc6
 * @date 2020-3-15
 */
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include <netinet/in.h>

#ifndef __TCPCLIENT_H__
#define __TCPCLIENT_H__
const int MESSAGE_LEN = 1024;

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
    void toconnect(const char* ip, int port);
	void run();
};

}//namespace tcpClient

#endif//__TCPCLIENT_H__
