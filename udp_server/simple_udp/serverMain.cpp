/**
 * serverMain.cpp (main)
 * 
 * this is a udp server
 *
 * @author zhyjc6
 * @date 2020-03-11
 */

#include "udpServer.h"


int main (int argc, char* argv[]) {
	//daemonize(); // make the server a background process
	udpServer::UdpServer server;
    
    server.startup(12345);
	server.run();

	return 0;
}
