/**
 * tcpServer.h
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-15
 */

#include <iostream>

#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <stdlib.h>
#include <fcntl.h>


#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

const int MESSAGE_LEN = 1024;
const int MAX_EVENTS = 20;
const int FD_SIZE = 1024;
const int TIME_OUT = 500;
const int MAX_PROCESS = 4;

namespace tcpServer{

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

		socklen_t addr_len = sizeof(struct sockaddr);

		int backlog; // the max num of the listening queue
		int flags;  //the flags of listenSocket
		
		struct epoll_event ev;
		struct epoll_event events[FD_SIZE];
		int epoll_fd;
		int epoll_number;
		int pid = -1;

		char buff[MESSAGE_LEN];

	public:
        void startup(int port, int back_log);
		void run();
};

}//namespace tcpServer

#endif//__TCPSERVER_H__
