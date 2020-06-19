/**
 * serverMain.cpp
 * this is a tcp server by cpp
 *
 * @author zhyjc6
 * @date 2020-3-10
 */

#include "tcpServer.h"

using namespace tcpServer;

int main() {
    // daemonize(); // make the server a background process
	TcpServer myserver;
    myserver.startup(12345, 10);
	myserver.run();	

	return 0;
}
