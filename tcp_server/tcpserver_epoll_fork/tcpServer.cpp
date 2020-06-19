/**
 * tcpServer.cpp
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-15
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
    fcntl(listenSocket, F_SETFL, flags|O_NONBLOCK);

    int nBuff = 1;
    if ( setsockopt(listenSocket,
                    SOL_SOCKET,
                    SO_REUSEADDR,
                    &nBuff,
                    sizeof(int)) < 0) {
        error_die("set socket error\n");
    }

    localaddr.sin_family = AF_INET;
    localaddr.sin_addr.s_addr = INADDR_ANY;
    localaddr.sin_port = htons(port);
    bzero(&localaddr.sin_zero, 8);
    this->backlog = back_log;
    
    if ( bind(listenSocket,
                (struct sockaddr*)&localaddr,
                addr_len) < 0 ) {
        error_die("bind error\n");
    }

    if ( listen(listenSocket, backlog) < 0 ) {
        error_die("listen error\n");
    }

    std::cout << "listening on port: " << port << std::endl;
}

void TcpServer::run() {
	for (int i = 0; i < MAX_PROCESS; i++) {
		if (pid != 0) { //One parent process creates multiple child processes
			pid = fork();
		}
	}
	if (pid == 0) {
		std::cout << "child process..." << std::endl;
		//create epoll
		epoll_fd = epoll_create(256);//the argument will be ignored
		ev.data.fd = listenSocket;
		ev.events = EPOLLIN;
		
		if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listenSocket, &ev) < 0) {
            error_die("epoll_ctl_add error\n");
		}

		while(1) {
			epoll_number = epoll_wait(epoll_fd, events, MAX_EVENTS, TIME_OUT);
			if (epoll_number < 0) {
                error_die("epoll_wait error\n");
			}

			for (int i = 0; i < epoll_number; i++) {
				if (events[i].data.fd == listenSocket) {
					acceptSocket = accept(listenSocket,
											(struct sockaddr*)&remoteaddr,
											&addr_len);
					if (acceptSocket < 0) {
                        error_die("accept error\n");
					}

					flags = fcntl(acceptSocket, F_GETFL, 0);
					fcntl(acceptSocket, F_SETFL, flags|O_NONBLOCK); //set no block
					
					ev.data.fd = acceptSocket;
					ev.events = EPOLLIN | EPOLLET;
					epoll_ctl(epoll_fd, EPOLL_CTL_ADD, acceptSocket, &ev);
					
					std::cout << "\na new client: " << acceptSocket << std::endl;
				} else if (events && EPOLLIN) {
					bzero(buff, sizeof(buff));
					int ret = recv(events[i].data.fd,
								   buff,
								   MESSAGE_LEN,
								   0
									);
					if (ret == -1) {
                        error_die("recv error\n");
					}
					if (ret == 0) {
						std::cout << "client " << events[i].data.fd << " disconnected." << std::endl;
						fcntl(epoll_fd, EPOLL_CTL_DEL, events[i].data.fd, &ev);
						close(ev.data.fd);
                        continue;
					}
					std::cout << "recv from client(" << events[i].data.fd << "): " << buff << std::endl;

					ret = send(events[i].data.fd,
							   buff,
							   ret,
							   0
								);
					if (ret == -1) {
                        error_die("send error\n");
					}
					std::cout << "send to client(" << events[i].data.fd << "): " << buff << std::endl;
				}
			}
		}
	} else {//pid != 0
		do {//wait until all child process are killed
			pid = waitpid(-1, NULL, 0);
		} while (pid != -1);
	}
}
}//namespace tcpServer
