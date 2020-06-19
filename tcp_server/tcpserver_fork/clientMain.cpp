/**
 * myclient.cpp
 *
 * this is a tcp client built with c++
 *
 * @author zhyjc6
 * @date 2020-3-10
 */


#include "tcpClient.h"

int main(int argc, char* argv[]) {
	tcpClient::TcpClient myclient;
	
	myclient.toConnect(12345);
	myclient.run();	

	return 0;
}
