/**
 * tcpClient.cpp
 *
 * this is a tcp client built with c++
 *
 * @author zhyjc6
 * @date 2020-3-10
 */


#include "tcpClient.h"
#include <arpa/inet.h>
#include <sys/socket.h>

namespace tcpClient {

void error_die(const char* err) {
    perror(err);
    std::cout << "error code: " << errno << std::endl;
    exit(1);
}
 
TcpClient::TcpClient() {
	conSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (conSocket < 0) {
        error_die("create socket error\n");
	}
}
TcpClient::~TcpClient() {
	close(conSocket);
}

void TcpClient::toconnect(const char* ip, int port) {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (connect(conSocket, (struct sockaddr*)&server_addr, sizeof(struct sockaddr)) < 0) {
        error_die("connect error\n");
    }
    std::cout << "connected." << std::endl;
}
void TcpClient::run() {
	while(1) {
		memset(sendbuf, 0, MESSAGE_LEN);
		fgets(sendbuf, MESSAGE_LEN, stdin);
		
		/*remove the '\n' at the end of sendbuf */
        int n = strlen(sendbuf);
        if (n > 1 && sendbuf[n-1] == '\n') {
        	sendbuf[n-1] = '\0';
        }
		if ( send(conSocket,
				  sendbuf,
				  strlen(sendbuf),
				  0) < 0 ) {
            error_die("send error\n");
        }
		std::cout << "client sent: " << sendbuf << std::endl;
		
		int ret = recv(conSocket,
				        recvbuf,
				        MESSAGE_LEN,
				        0);
		if (ret == -1) {
            error_die("recv error\n");
		}
		recvbuf[ret] = '\0';
		std::cout << "client recv: " << recvbuf << std::endl;
	}
}

}//namespace tcpClient
