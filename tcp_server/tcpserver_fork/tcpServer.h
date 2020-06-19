/**
 * tcpServer.h
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-10
 */

#include <iostream>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <asm-generic/socket.h>
#include <cstdio>

#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#define MESSAGE_LEN 1024

namespace tcpServer{

void error_die(const char* err);
void daemonize();

class TcpServer{
	public:
		TcpServer();
		~TcpServer();

	private:
		int listenSocket;
		int acceptSocket;
		struct sockaddr_in localaddr;
		struct sockaddr_in remoteaddr;

		int backlog;
		char buff[MESSAGE_LEN];

	public:
		void startup(int port = 0, int backlog = 10); 
		void run();
};

}//namespace tcpServer

#endif//__TCPSERVER_H__
