/**
 *
 * udpClient.cpp
 * this is a udp server
 *
 * @author zhyjc6
 * @date 2020-03-11
 */

#include "udpClient.h"

namespace udpClient {

void error_die(const char* err) {
    std::cout << err << std::endl;
    std::cout << "error code: " << errno << std::endl;
    exit(1);
}

UdpClient::UdpClient() {
	this->cSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if (this->cSocket < 0) {
        error_die("create socket error\n");
    }
	std::cout << "create socket success" << std::endl;
}
UdpClient::~UdpClient () {
	close(this->cSocket);
}

void UdpClient::setport(int port) {
	bzero(&this->targetaddr, sizeof(this->targetaddr));
	this->targetaddr.sin_family = AF_INET;
	this->targetaddr.sin_port = htons(port);
	this->targetaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	std::cout << "set target server:127.0.0.1: " << port << std::endl; 
}

void UdpClient::run() {
	int len;
	socklen_t addrlen = sizeof(struct sockaddr_in);
	while (1) {
		bzero(this->sendbuf, sizeof(this->sendbuf));
		fgets(this->sendbuf, MESSAGE_LEN, stdin);
		int sendLen = strlen(this->sendbuf);
		if (sendLen > 1 && this->sendbuf[sendLen-1] == '\n') {
			this->sendbuf[sendLen-1] = '\0';
			std::cout << "your input: " << this->sendbuf << std::endl;
		}
		
		len = sendto(this->cSocket,
					 this->sendbuf,
					 strlen(this->sendbuf),
					 0,
					 (struct sockaddr*)&this->targetaddr,
					 addrlen
					);
		if (len < 0) {
            error_die("client send error\n");
		} else {
			std::cout << "client send: " << this->sendbuf << std::endl;
		}
		
		len = recvfrom(this->cSocket,
					   this->recvbuf,
					   MESSAGE_LEN,
					   0,
					   (struct sockaddr*)&this->targetaddr,
					   &addrlen
					  );
		if (len < 0) {
            error_die("client recv error\n");
		} else {
			this->recvbuf[len] = '\0';
			std::cout << "client recv: " << this->recvbuf << std::endl;
		}
	} //while (1) 
}

}//namespace udpClient
