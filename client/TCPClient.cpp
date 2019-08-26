#include "TCPClient.h"
#include "exceptions.h"

TCPClient::TCPClient()
	: sock(-1)
	, port(0)
	, address("")
{

}
TCPClient::~TCPClient()
{
	Close();
}
void TCPClient::Setup(std::string address , int port)
{
  	if(sock == -1) {
		sock = socket(AF_INET, SOCK_STREAM , 0);
		if (sock == -1) {
			throw SocketException("Could not create socket");
		}
	}
	server.sin_addr.s_addr = inet_addr(address.c_str());
  	server.sin_family = AF_INET;
  	server.sin_port = htons( port );
  	if (connect(sock , reinterpret_cast<struct sockaddr *>(&server) , sizeof(server)) < 0) {
		throw SocketException("Connect failed.");
  	}
}

void TCPClient::Send(std::string data)
{
	if(sock == -1) {
		throw SocketException("Connect failed.");
	}
	
	static int count (0);
	count++;
	std::string msgToSend(std::to_string(count) + "\t" + data);
	if(send(sock, msgToSend.c_str(), msgToSend.length(), 0) < 0) {
		throw SocketException("Send failed : " + data);
	}
}

std::string TCPClient::Receive(int size)
{
	std::string buf;
	buf.resize(size);
	if(recv(sock, &buf.front(), size, 0) < 0) {
	    throw SocketException("Receive failed!");
  	}
  	return buf;
}
std::string TCPClient::Read()
{
  	char buffer[1] = {};
  	std::stringstream reply;
  	while (buffer[0] != '\n') {
		if( recv(sock , buffer , sizeof(buffer) , 0) < 0)
		{
			std::cout << "receive failed!" << std::endl;
			return nullptr;
		}
		reply << buffer[0];
	}
	return reply.str();
}
void TCPClient::Close()
{
    close( sock );
}
