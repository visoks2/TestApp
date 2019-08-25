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

#define MAXPACKETSIZE 40960
#define MAX_CLIENT 1000
//#define CODA_MSG 4

struct descript_socket{
	int socket     = -1;
	std::string ip      = "";
	int id         = -1; 
	std::string message;
	bool enable_message_runtime = false;
};

class TCPServer
{
	public:
	TCPServer(){};
	~TCPServer(){
		close(sockfd);
	}
	void Close(){
		close(sockfd);
	}
	// TODO: move to construct
	void Setup(int port, std::vector<int>& opts);

	std::vector<struct descript_socket*> getMessage();
	void AcceptNewClients();
	void Send(std::string msg, int id);
	void detach(int id);
	void clean(int id);
        bool is_online();
	std::string get_ip_addr(int id);
	int get_last_closed_sockets();

	private:
	int sockfd, n, pid;
	struct sockaddr_in serverAddress;
	struct sockaddr_in clientAddress;

	static std::vector<struct descript_socket*> newsockfd;
	static char msg[ MAXPACKETSIZE ];
	static std::vector<struct descript_socket*> Message;

	static bool isonline;
	static int last_closed;
	static int num_client;
	static std::mutex mt;
	static void Task(descript_socket* desc);
};
