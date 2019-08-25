#include "TCPServer.h" 
#include "exceptions.h" 
#include <memory> 
#include <thread>


char TCPServer::msg[MAXPACKETSIZE];
int TCPServer::num_client;
int TCPServer::last_closed;
bool TCPServer::isonline;
std::vector<struct descript_socket*> TCPServer::Message;
std::vector<struct descript_socket*> TCPServer::newsockfd;
std::mutex TCPServer::mt;

void TCPServer::Task(descript_socket* desc)
{
	std::cout << "open client[ id:"	<< desc->id
			  << " ip:"				<< desc->ip 
			  << " socket:"			<< desc->socket
			  << " send:"			<< desc->enable_message_runtime 
			  << " ]" << std::endl;
	while(1)
	{
		ssize_t recivedSize = recv(desc->socket, msg, MAXPACKETSIZE, 0);
		if(recivedSize != -1) 
		{
			if(recivedSize == 0)
			{
			   isonline = false;
			   std::cout << "close client[ id:"	<< desc->id 
						 << " ip:"				<< desc->ip 
						 << " socket:"			<< desc->socket
						 << " ]" << std::endl;
			   last_closed = desc->id;
			   close(desc->socket);

			   int id = desc->id;
			   auto new_end = std::remove_if(newsockfd.begin(), newsockfd.end(), [id](descript_socket *device) { return device->id == id; });
			   newsockfd.erase(new_end, newsockfd.end());

			   if(num_client>0) num_client--;
			   break;
			}
			msg[recivedSize]=0;
			desc->message = std::string(msg);
			std::lock_guard<std::mutex> guard(mt);
			Message.push_back(desc);
		}
		usleep(600);
	}
}

void TCPServer::Setup(int port, std::vector<int>& opts)
{
	isonline = false;
	int opt = 1;
	last_closed = -1;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
 	memset(&serverAddress, 0, sizeof(serverAddress));

	for(unsigned int i = 0; i < opts.size(); i++) {
		if( (setsockopt(sockfd, SOL_SOCKET, opts.size(), &opt, sizeof(opt))) < 0 ) {
	std::cout << " Setup 1" << std::endl;
			throw SocketException("setsockopt failed"); 
		}
	}

	serverAddress.sin_family      = AF_INET;
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddress.sin_port        = htons(port);

	if((bind(sockfd,(struct sockaddr *)&serverAddress, sizeof(serverAddress))) < 0){
		throw SocketException("bind failed"); 
	}
	
 	if(listen(sockfd,5) < 0){
		throw SocketException("listen failed"); 
	}
	num_client = 0;
	isonline = true;
}

void TCPServer::AcceptNewClients()
{
	socklen_t sosize    = sizeof(clientAddress);
	struct descript_socket* so = new descript_socket;
	so->socket          = accept(sockfd,(struct sockaddr*)&clientAddress,&sosize);
	so->id              = num_client;
	so->ip              = inet_ntoa(clientAddress.sin_addr);
	newsockfd.push_back( so );
	std::cout << "New Client! [id:" << newsockfd[num_client]->id 
			  << ", ip:" 			<< newsockfd[num_client]->ip
			  << ", handle:" 		<< newsockfd[num_client]->socket 
			  << " ]" << std::endl;
	std::thread t1(Task, newsockfd[num_client]);
	t1.detach();
	isonline=true;
	num_client++;
}

std::vector<struct descript_socket*> TCPServer::getMessage()
{
	std::lock_guard<std::mutex> guard(mt);
	return Message;
}

void TCPServer::Send(std::string msg, int id)
{
	send(newsockfd[id]->socket,msg.c_str(),msg.length(),0);
}

int TCPServer::get_last_closed_sockets()
{
	return last_closed;
}

void TCPServer::clean(int id)
{
	Message[id]->message.clear();
	memset(msg, 0, MAXPACKETSIZE);
}

std::string TCPServer::get_ip_addr(int id)
{
	return newsockfd[id]->ip;
}

bool TCPServer::is_online() 
{
	return isonline;
}

void TCPServer::detach(int id)
{
	close(newsockfd[id]->socket);
	newsockfd[id]->ip = "";
	newsockfd[id]->id = -1;
	newsockfd[id]->message = "";
} 


