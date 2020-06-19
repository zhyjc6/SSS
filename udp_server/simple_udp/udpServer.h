/**
 * udpServer.h
 *
 * this is a udp server
 *
 * @author zhyjc6
 * @date 2020-03-11
 */


#include <iostream>

#include <netinet/in.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__

namespace udpServer {

class UdpServer {
public:
	UdpServer();
	~UdpServer();

private:
	int sSocket;
	char *buf;
	struct sockaddr_in localaddr;

public:
    void startup(int port);
	void run();
};

}//namespace udpServer

#endif//__UPDSERVER_H__
