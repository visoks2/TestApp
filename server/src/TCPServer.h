#pragma once

#include <iostream>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <thread>
#include <algorithm>
#include <cctype>
#include <map>
#include <memory>
#include <atomic>
#include <thread>


#include "socket_description.h"
#include "MessagesHandler.h"


namespace Server {
	constexpr int MAXPACKETSIZE (1024);
	constexpr int 				SERVER_PORT			(5050);
	const	  std::vector<int> 	SOCKET_OPTIONS 		{ SO_REUSEPORT, SO_REUSEADDR };
	constexpr int 			   	REFRESH_INTERVAL	{100};
	constexpr int 			   	SEND_INTERVAL		{100};
	constexpr timeval 			SELECT_TIMEOUT		{0, 250000};
	
	class TCPServer
	{

	public:
		TCPServer();
		~TCPServer();

		void AcceptNewClients();
		static bool is_online();

		std::vector<std::thread> threads;
		std::atomic<bool> keepRunning;
	private:
		static void welcomeLoop(TCPServer * aServer);
		std::thread acceptNewClients;
		bool isAcceptingNewClients;

		std::thread receiveThread;


		int sockfd;
		struct sockaddr_in serverAddress;
		struct sockaddr_in clientAddress;

		std::map<int, socket_description> newsockfd;

		static bool isOnline;
		static void listenLoop(TCPServer * aServer, const int id);
		MessagesHandler messagesHandler;
	};
}
