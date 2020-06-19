/**
 * tcpServer.cpp
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-10
 */

#include "tcpServer.h"

namespace tcpServer {

void error_die(const char* err) {
    perror(err);
    std::cout << "error code: " << errno << std::endl;
    exit(1);
}

void daemonize() {
	int fd;
	pid_t pid;
	
	//create a child process
	if ((pid = fork()) < 0) {
		std::cout << "can't create child process!" << std::endl;
	} else {
		if (pid != 0) {
			exit(0);
		}
	}
	if (chdir("/") < 0) {
		std::cout << "can't change dir!" << std::endl;
		exit(-1);
	}
	setsid();
	fd = open("dev/null", O_RDWR);

	dup2(fd, STDIN_FILENO);
	dup2(fd, STDOUT_FILENO);
	dup2(fd, STDERR_FILENO);
}

TcpServer::TcpServer() {
    listenSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket < 0) {
        error_die("create socket error\n");
    }
}
TcpServer::~TcpServer() {
	close(listenSocket);
}


void TcpServer::startup(int port, int backlog) {
    localaddr.sin_family = AF_INET;
	localaddr.sin_addr.s_addr = INADDR_ANY;
    localaddr.sin_port = htons(port);
	bzero(&(localaddr.sin_zero), 8);
   
    this->backlog = backlog;
    
    int flag = 1;
    if (setsockopt(listenSocket, SOL_SOCKET, 
                            SO_REUSEADDR, &flag, sizeof(int)) < 0) {
        error_die("set socket options error\n");
    }

    if (bind(listenSocket, (struct sockaddr*)&localaddr,
                sizeof(struct sockaddr)) < 0) {
        error_die("socket bind error\n");
    }

    if (listen(listenSocket, backlog) < 0) {
        error_die("listen error\n");
    }

    std::cout << "server is listenning " << port << std::endl;

}

void TcpServer::run() {
	socklen_t sizeofSockaddr = sizeof(struct sockaddr*);
	while(1) {
		std::cout << "waiting for connecting..." << std::endl;
		acceptSocket = accept(listenSocket,
							  (struct sockaddr*)&remoteaddr,
							  &sizeofSockaddr);
		if (acceptSocket == -1) {
            error_die("accept error\n");
		}
		std::cout << "connected by client." << std::endl;		
		
        pid_t pid = fork(); // create child process
		if (pid < 0) {
            error_die("create child process error\n");
		}
		if (pid == 0) { //child process
			close(listenSocket); //useless
			while (1) {
				memset(buff, 0, MESSAGE_LEN);
				int ret = recv(acceptSocket,
							   (void*)buff,
							   MESSAGE_LEN,
							   0
								);
				if (ret == -1) {
                    error_die("recv error\n");
				}
				if (ret == 0) {
                    error_die("client disconnected\n");
				}
				std::cout << "server recv: " << buff << std::endl;
			
				ret = send(acceptSocket,
						   (void*)buff,
						   MESSAGE_LEN,
						   0
							);
				if (ret == -1) {
                    error_die("send error\n");
				}
				std::cout << "server send: " << buff << std::endl;
			
			}
			close(acceptSocket);
			exit(0);
		}
		if (pid > 0) { //father process
			signal(SIGCHLD,SIG_IGN); // avoid defunct process
			close(acceptSocket);
		}
	}
}

}//namespace tcpServer
