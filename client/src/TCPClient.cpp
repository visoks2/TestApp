#include "TCPClient.h"
#include "Exceptions.h"

using namespace Client;

TCPClient::TCPClient(std::string address , int port)
	: sock(-1)
	, port(0)
	, address("")
{
	if(sock == -1) {
		sock = socket(AF_INET, SOCK_STREAM , 0);
		if (sock == -1) {
			throw Common::SocketException("Could not create socket");
		}
	}
	server.sin_addr.s_addr = inet_addr(address.c_str());
  	server.sin_family = AF_INET;
  	server.sin_port = htons( port );
  	if (connect(sock , reinterpret_cast<struct sockaddr *>(&server) , sizeof(server)) < 0) {
		throw Common::SocketException("Connect failed.");
  	}
}
TCPClient::~TCPClient()
{
	Close();
}

void TCPClient::Send(gpb::Message & message)
{
	if(sock == -1) {
		throw Common::SocketException("Connect failed.");
	}
	
	std::string msgToSend(message.SerializeAsString() + '\0');
	if(send(sock, msgToSend.c_str(), msgToSend.length(), 0) < 0) {
		throw Common::SocketException("Send failed");
	}
}

void TCPClient::Create(std::string&& aId, std::string&& aName, std::string&& aPsw)
{
	gpb::Message message;
	message.set_action(gpb::Message_Action::Message_Action_CREATE);
	auto* record = message.mutable_record();
	record->set_id(aId);
	record->set_username(aName);
	record->set_password(aPsw);
	Send(message);

}

void TCPClient::Delete(std::string&& aId, std::string&& aName)
{
	gpb::Message message;
	message.set_action(gpb::Message_Action::Message_Action_DELETE);
	auto* record = message.mutable_record();
	record->set_id(aId);
	record->set_username(aName);
	Send(message);
}

std::string TCPClient::Receive(int size)
{
	std::string buf;
	buf.resize(size);
	if(recv(sock, &buf.front(), size, 0) < 0) {
	    throw Common::SocketException("Receive failed!");
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
