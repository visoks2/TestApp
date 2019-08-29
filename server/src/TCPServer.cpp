#include "TCPServer.h" 
#include "Exceptions.h" 
#include <sstream>

using namespace Server;

bool TCPServer::isOnline;


TCPServer::TCPServer()
{
	keepRunning = true;
	isOnline = false;
	int opt = 1;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	memset(&serverAddress, 0, sizeof(serverAddress));

	for(unsigned int i = 0; i < SOCKET_OPTIONS.size(); i++) {
		if ((setsockopt(sockfd, SOL_SOCKET, SOCKET_OPTIONS.size(), &opt, sizeof(opt))) < 0) {
			throw Common::SocketException("setsockopt failed"); 
		}
	}

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port        = htons(SERVER_PORT);

	if ((bind(sockfd, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress))) < 0) {
		throw Common::SocketException("bind failed"); 
	}
	
	if(listen(sockfd,5) < 0) {
		throw Common::SocketException("listen failed"); 
	}
	isOnline = true;
}

void TCPServer::listenLoop(TCPServer * aServer, const int id)
{
	auto& desc = aServer->newsockfd.at(id);
	std::cout << "New client \t" << desc << std::endl;
	while(aServer->keepRunning)
	{
		std::stringstream ss;
		char buffer[1] = {'?'};
		while (buffer[0] != 0) {
			ssize_t receivedSize = recv(desc.socket, buffer, sizeof(buffer), 0);
			if (receivedSize != -1) 
			{
				if(receivedSize == 0) {
					std::cout << "Client left \t" << desc << std::endl;
					aServer->isOnline = false;
					close(desc.socket);
					aServer->newsockfd.erase(id);
					return;
				}
				ss << buffer[0];
			}
		}
		std::string str(ss.str());
		str.erase(str.length() - 1);
		std::string response = aServer->messagesHandler.ProcessMessage(str);
		send(desc.socket, response.c_str(), response.length(), 0);

		// char msg[ MAXPACKETSIZE ];
		// ssize_t receivedSize = recv(desc.socket, msg, MAXPACKETSIZE, 0);
		// if(receivedSize != -1) 
		// {
		// 	if(receivedSize == 0) {
		// 		std::cout << "Client left \t" << desc << std::endl;
		// 		aServer->isOnline = false;
		// 		close(desc.socket);
		// 		aServer->newsockfd.erase(id);
		// 		return;
		// 	}
		// 	msg[receivedSize] = 0;
		// 	std::string response = aServer->messagesHandler.ProcessMessage(std::string(msg));
		// 	send(desc.socket, response.c_str(), response.length(), 0);
		// }		
		// std::this_thread::sleep_for(std::chrono::milliseconds(REFRESH_INTERVAL));
	}
}


TCPServer::~TCPServer()
{
	isAcceptingNewClients = false;
	acceptNewClients.join();
	keepRunning = false;
	for (auto &&t : threads) {
		t.join();
	}
	// receiveThread.join();
	
	close(sockfd);
}
void TCPServer::AcceptNewClients()
{
	isAcceptingNewClients = true;
	acceptNewClients = std::thread(welcomeLoop, this);
}

void TCPServer::welcomeLoop(TCPServer * aServer)
{
	while(aServer->isAcceptingNewClients) {
		// check if any1 is trying to connect w/ timeout = SELECT_TIMEOUT
		timeval to = SELECT_TIMEOUT;
		fd_set rset;
		FD_ZERO(&rset);  
        FD_SET(aServer->sockfd, &rset); 
		int selectVal = select(aServer->sockfd+1, &rset, NULL, NULL, &to);
		if (!FD_ISSET(aServer->sockfd, &rset)) continue;

		static int clientID(0);
		clientID++;

		// hurray, got client, accept it
		socklen_t sosize    = sizeof(clientAddress);
		socket_description so (
			accept(aServer->sockfd,(struct sockaddr*)&aServer->clientAddress,&sosize),
			inet_ntoa(aServer->clientAddress.sin_addr),
			clientID
		);
		aServer->newsockfd[clientID] = std::move(so);

		// make some thread to listen to this client
		std::thread t(listenLoop, aServer, clientID);
		aServer->threads.push_back(std::move(t));
	}
}

bool TCPServer::is_online() 
{
	return isOnline;
}











