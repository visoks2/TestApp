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
#include <mutex>
#include "socket_description.h"
#include <map>

#define MAXPACKETSIZE 1024
#define MAX_CLIENT 1000
//#define CODA_MSG 4
namespace Server{
	class TCPServer
	{
	public:
		TCPServer(int port, std::vector<int> opts);
		~TCPServer(){
			close(sockfd);
		}
		void Close(){
			close(sockfd);
		}
		// TODO: move to construct
		


		static void Send(std::string msg, int id);
		static void sendTime(socket_description* desc);

		void AcceptNewClients();
		void Receive();
		static bool is_online();

	// private:
		static void acceptLoop(int sockfd, struct sockaddr_in clientAddress);
		static void receiveLoop();


		int sockfd, n, pid;
		struct sockaddr_in serverAddress;
		struct sockaddr_in clientAddress;

		static std::map<int, socket_description> newsockfd;

		static bool isOnline;
		static void listenLoop(const int id);
	};
}
