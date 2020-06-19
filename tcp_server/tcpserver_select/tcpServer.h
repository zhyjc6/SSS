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
#include <sys/wait.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>

#include <asm-generic/socket.h>
#include <ctime>
#include <iterator>
#include <netinet/in.h>


#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__

const int MESSAGE_LEN = 1024;
const int FD_SIZE = 1024;

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
		socklen_t addr_len;

		int backlog; // the max num of the listening queue
		int flags;  //the flags of listenSocket
		int max_fd;
		int accept_fds[FD_SIZE];
		int events;
		fd_set fd_sets;
		int curpos = -1;
		
		struct timeval timeout;
		
		char buff[MESSAGE_LEN];

	public:
        void startup(int port, int backlog);
		void run();
};

}//namespace tcpServer

#endif//__TCPSERVER_H__
