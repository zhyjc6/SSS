/**
 * udpClient.h
 *
 * this is a udp server
 *
 * @author zhyjc6
 * @date 2020-03-11
 */

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <functional>
#include <system_error>


#ifndef __UDPCLIENT_H__
#define __UDPCLIENT_H__

#define MESSAGE_LEN 1024


namespace udpClient {

class UdpClient {
public:
	UdpClient();
	~UdpClient();

private:
	int cSocket;
	struct sockaddr_in targetaddr;
	char sendbuf[MESSAGE_LEN];
	char recvbuf[MESSAGE_LEN];

public:
	void setport(int port);
	void run();
};

}//namespace udpClient

#endif//__UDPCLIENT_H__
