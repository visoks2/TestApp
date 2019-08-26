#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>
#include <algorithm>
#include <cctype>
#include <map>


#include "socket_description.h"


namespace Server{
	constexpr int MAXPACKETSIZE (1024);
	
	class TCPServer
	{
	public:
		TCPServer(int port, std::vector<int> opts);
		~TCPServer(){
			close(sockfd);
		}
		static void sendTime(socket_description* desc);

		void AcceptNewClients();
		void Receive();
		static bool is_online();

	private:
		static void acceptLoop(int aSockfd, struct sockaddr_in aClientAddress);
		static void receiveLoop();


		int sockfd;
		struct sockaddr_in serverAddress;
		struct sockaddr_in clientAddress;

		static std::map<int, socket_description> newsockfd;

		static bool isOnline;
		static void listenLoop(const int id);
	};
}
