/**
 * myclient.cpp
 *
 * this is a tcp client built with c++
 *
 * @author zhyjc6
 * @date 2020-3-15
 */

#include "tcpClient.h"

int main(int argc, char* argv[]) {
	tcpClient::TcpClient myclient;
	
	myclient.toconnect("127.0.0.1", 12345);
    myclient.run();	

	return 0;
}
