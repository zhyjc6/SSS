/**
 * tcpServer.cpp
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-10
 */

#include "tcpServer.h"
#include <ctime>
#include <sys/socket.h>

namespace tcpServer {

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

void error_die(const char* err) {
    perror(err);
    std::cout << "error code: " << errno << std::endl;
    exit(1);
}

TcpServer::TcpServer() {
	addr_len = sizeof(struct sockaddr);
	//set timeout 3.5s
    timeout.tv_sec = 3;
    timeout.tv_usec = 500000;

	for (int i = 0; i < FD_SIZE; i++) { // initial accept_fds
		accept_fds[i] = -1;
	}

	listenSocket = socket(AF_INET, SOCK_STREAM, 0); // create listen socket
	if (listenSocket == -1) {
        error_die("create socket error\n");
	}
}

TcpServer::~TcpServer() {
	close(listenSocket);
}


void TcpServer::startup(int port, int back_log) {
    flags = fcntl(listenSocket, F_GETFL, 0);
    fcntl(listenSocket, F_SETFL, flags | O_NONBLOCK);
    max_fd = listenSocket;

    int flag = 1;
    if ( setsockopt(listenSocket,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &flag,
                    sizeof(int) ) < 0 ) {
        error_die("set socket error\n");
    }

    localaddr.sin_family = AF_INET;
    localaddr.sin_port = htons(port);
    localaddr.sin_addr.s_addr = INADDR_ANY;
    bzero(&localaddr.sin_zero, 8);

    backlog = back_log;

    if (bind(listenSocket, (struct sockaddr*)&localaddr, addr_len) < 0) {
        error_die("bind error\n");
    }
    if (listen(listenSocket, backlog) < 0) {
        error_die("listen error\n");
    }

    std::cout << "listening on port: " << port << std::endl;
}

void TcpServer::run() {
	while(1) {
		FD_ZERO(&fd_sets); //clear sets
		FD_SET(listenSocket, &fd_sets); // add listenSocket to sets

		for (int i = 0; i < FD_SIZE; i++) {
			if (accept_fds[i] != -1) {
				if (max_fd < accept_fds[i]) {
					max_fd = accept_fds[i]; //find the max fd
				}
				FD_SET(accept_fds[i], &fd_sets); //add accept_fds[i] to sets
			}
		}
		// Traverse all fd
		events = select(max_fd+1,
						&fd_sets,
						NULL,
						NULL,
						&timeout);
		if (events < 0) {
            error_die("select error\n");
		} else if (events == 0) {
			//std::cout << "timeout! continue." << std::endl;
			continue;
		} else { //events > 0, listenSocket create the event(new accept socket)
			std::cout << "event: " << events << std::endl;
			if (FD_ISSET(listenSocket, &fd_sets)) { // a new connect
				int i;
				for (i = 0; i < FD_SIZE; i++) { //find the first empty position
					if (accept_fds[i] == -1) {
						curpos = i;
						break;
					}
				}
				if (i == FD_SIZE) {
					std::cout << "the connection is full! continue." << std::endl;
					continue;
				}
				acceptSocket = accept(listenSocket, //create a new socket for new connection
									    (struct sockaddr*)&remoteaddr,
										&addr_len);
				if (acceptSocket == -1) {
                    error_die("accept error\n");
				}
				std::cout << "\nconnected." << std::endl;
                std::cout << "a new client: " << acceptSocket << std::endl;

				flags = fcntl(acceptSocket, F_GETFL, 0);
				fcntl(acceptSocket, F_SETFL, flags | O_NONBLOCK); //no block

				accept_fds[curpos] = acceptSocket;
			}

			for (int i = 0; i < FD_SIZE; i++) {
				if (accept_fds[i] != -1 && FD_ISSET(accept_fds[i], &fd_sets)) {
					bzero(buff, sizeof(buff));
					int ret = recv(accept_fds[i],
								   buff,
								   MESSAGE_LEN,
								   0 );
					if (ret == -1) {
                        error_die("recv error\n");
					}
					if (ret == 0) {
                        close(accept_fds[i]);
                        std::cout << "client disconnected." << std::endl;
                        std::cout << "closed client: " << accept_fds[i] << std::endl;
                        accept_fds[i] = -1;
                        break;
					}
					std::cout << "server recv: " << buff << std::endl;
	
					ret = send(accept_fds[i],
							   buff,
							   ret,
							   0 );
					if (ret == -1) {
                        error_die("send error\n");
					}
					std::cout << "server send: " << buff << std::endl;
				}
			}
		}

	}
}
}//namespace tcpServer
