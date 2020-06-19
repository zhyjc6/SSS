/**
 * serverMain.cpp
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-10
 */

#include "tcpServer.h"

int main(int argc, char* argv[]) {
    // tcpServer::daemonize(); // make the server a background process
	tcpServer::TcpServer myserver;
	
    myserver.startup(12345, 2);
    myserver.run();	

	return 0;
}
