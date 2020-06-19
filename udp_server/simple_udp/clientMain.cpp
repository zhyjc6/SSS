/**
  * clientMain.cpp (main)
  *
  * this is a udp server
  *
  * @author zhyjc6
  * @date 2020-03-11
  */

#include "udpClient.h"

int main (int argc, char* argv[]) {
	udpClient::UdpClient client;
	
	client.setport(12345);
	client.run();

	return 0;
}
