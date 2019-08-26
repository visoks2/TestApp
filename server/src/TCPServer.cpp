#include "TCPServer.h" 
#include "exceptions.h" 
#include <memory> 
#include <thread>
#include <map>

using namespace Server;

constexpr int 			   	REFRESH_INTERVAL	{5000};
constexpr int 			   	SEND_INTERVAL		{5000};


bool TCPServer::isOnline;
std::map<int, socket_description> TCPServer::newsockfd;


void TCPServer::listenLoop(const int id)
{
	auto& desc = newsockfd.at(id);
	std::cout << "New client " << desc << std::endl;
	while(1)
	{
		char msg[ MAXPACKETSIZE ];
		ssize_t receivedSize = recv(desc.socket, msg, MAXPACKETSIZE, 0);
		if(receivedSize != -1) 
		{
			if(receivedSize == 0) {
				std::cout << "Client left " << desc << std::endl;
				isOnline = false;
				close(desc.socket);
				newsockfd.erase(id);
				return;
			}
			msg[receivedSize] = 0;
			desc.message = std::string(msg);
		}
		usleep(REFRESH_INTERVAL);
	}
}

TCPServer::TCPServer(int port, std::vector<int> opts)
{
	isOnline = false;
	int opt = 1;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	memset(&serverAddress, 0, sizeof(serverAddress));

	for(unsigned int i = 0; i < opts.size(); i++) {
		if ((setsockopt(sockfd, SOL_SOCKET, opts.size(), &opt, sizeof(opt))) < 0) {
			throw SocketException("setsockopt failed"); 
		}
	}

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port        = htons(port);

	if ((bind(sockfd, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress))) < 0) {
		throw SocketException("bind failed"); 
	}
	
	if(listen(sockfd,5) < 0) {
		throw SocketException("listen failed"); 
	}
	isOnline = true;
}

void TCPServer::AcceptNewClients()
{
	std::thread t(acceptLoop, sockfd, clientAddress);
	t.detach();
}

void TCPServer::acceptLoop(int aSockfd, struct sockaddr_in aClientAddress)
{
	while(1) {
		static int clientID(0);
		clientID++;
		
		socklen_t sosize    = sizeof(clientAddress);
		socket_description so (
			accept(aSockfd,(struct sockaddr*)&aClientAddress,&sosize),
			inet_ntoa(aClientAddress.sin_addr),
			clientID
		);
		newsockfd[clientID] = std::move(so);
		std::thread t(listenLoop, clientID);
		t.detach();
		isOnline=true;
		usleep(REFRESH_INTERVAL);
	}
}
void TCPServer::Receive()
{
	std::thread thread_accept(receiveLoop);
	thread_accept.detach();
}

bool TCPServer::is_online() 
{
	return isOnline;
}
void TCPServer::receiveLoop()
{
	while(1)
	{
		for (auto &&sock : newsockfd)
		{
			
			if(!sock.second.message.empty())
			{
				std::cout << "received " << sock.second << std::endl;
				sock.second.message.clear();

				sendTime(&sock.second);
			}
		}
		usleep(REFRESH_INTERVAL);
	}
}

void TCPServer::sendTime(socket_description* desc) {
		if(!is_online()) {
			std::cout << "offline!" << std::endl;
			return;
		}
		std::time_t t = std::time(0);
		std::tm* now = std::localtime(&t);
		int hour = now->tm_hour;
		int min  = now->tm_min;
		int sec  = now->tm_sec;

		std::string date = 
			    std::to_string(now->tm_year + 1900) + "-" +
			    std::to_string(now->tm_mon + 1)     + "-" +
			    std::to_string(now->tm_mday)        + " " +
			    std::to_string(hour)                + ":" +
			    std::to_string(min)                 + ":" +
			    std::to_string(sec)                 + "\r\n";
		// usleep(REFRESH_INTERVAL);
			std::cout << "sending " << date << std::endl;
		send(desc->socket,date.c_str(),date.length(),0);
			std::cout << "sent" << std::endl;
}











