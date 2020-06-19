/**
 * udpServer.cpp
 * this is a udp server
 *
 * @author
 * @date 2020-03-11
 */
#include "udpServer.h"

#define MESSAGE_LEN 102

namespace udpServer {

void daemonize() {
	pid_t pid = fork();
	if (pid < 0) {
		std::cout << "can't create child process!" << std::endl;
		exit(-1);
	} else if (pid > 0) {
		exit(0);
	} else {
		if (chdir("/") < 0) {
			std::cout << "can't change dir!" << std::endl;
			exit(-1);
		}
		setsid();
		int fd = open("dev/null", O_RDWR);
		
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
	}
}


void error_die(const char* err) {
    perror(err);
	std::cout << "error code: " << errno << std::endl;
    exit(1);
}

UdpServer::UdpServer() {
	this->sSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(this->sSocket < 0) {
        error_die("create socket error\n");
	}
	std::cout << "creating socket successful." << std::endl;
	this->buf = new char(MESSAGE_LEN);
	if (this->buf) {
		std::cout << "buff len:" << MESSAGE_LEN << std::endl;
	} else {
		std::cout << "failed to new memory to buf!" << std::endl;
	}
}
UdpServer::~UdpServer() {
	if (this->sSocket >= 0) {
		close(this->sSocket);
		std::cout << "close socket successful." << std::endl;
	}
	if (this->buf) {
		delete this->buf;
	}
}
void UdpServer::startup(int port) {
    bzero(&localaddr, sizeof(localaddr));
    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(port);
    localaddr.sin_addr.s_addr = 0; //INADDR_ANY;

    if (bind(sSocket, (struct sockaddr*)&localaddr, sizeof(struct sockaddr)) < 0) {
       error_die("bind error\n"); 
    }
    std::cout << "server open port:" << port << std::endl;
}

void UdpServer::run() {
	int len;
	while (1) {
		std::cout << std::endl;
		bzero(this->buf, sizeof(this->buf));
		int addrlen = sizeof(struct sockaddr_in);
		
		len = recvfrom(sSocket, buf, sizeof(buf), 0, 
                        (struct sockaddr*)&localaddr,
					    (socklen_t*)&addrlen
						);
		if (len < 0) {
			error_die("server recvfrom error\n");
		} else if (len == 0) {
			std::cout << "client disconnect." << std::endl;
			continue;
		} else {
			std::cout << "server recv: " << this->buf << std::endl;
		}

		len = sendto(this->sSocket,
					 this->buf,
					 len,
					 0,
					 (struct sockaddr*)&this->localaddr,
					 addrlen
					);
		if (len < 0) {
			error_die("server sendto error\n");
		} else {
			std::cout << "server send: " << this->buf << std::endl;
		}
		
	}
}

}//namespace udpServer
