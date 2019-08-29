#include "TCPClient.h"
#include "Exceptions.h"
#include "Common.h"

using namespace Client;

TCPClient::TCPClient(std::string aAddress, int aPort)
	: sockedFd(-1)
{
	if(sockedFd == -1) {
		sockedFd = socket(AF_INET, SOCK_STREAM , 0);
		if (sockedFd == -1) {
			throw Common::SocketException("Could not create socket");
		}
	}
	server.sin_addr.s_addr = inet_addr(aAddress.c_str());
  	server.sin_family = AF_INET;
  	server.sin_port = htons( aPort );
  	if (connect(sockedFd , reinterpret_cast<struct sockaddr *>(&server) , sizeof(server)) < 0) {
		throw Common::SocketException("Connect failed.");
  	}
	setsockopt(sockedFd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&SELECT_TIMEOUT, sizeof(SELECT_TIMEOUT));

}

TCPClient::~TCPClient()
{
	Close();
}

void TCPClient::Send(gpb::Message & aMessage)
{
	if(sockedFd == -1) {
		throw Common::SocketException("Connect failed.");
	}
	
	std::string msgToSend(aMessage.SerializeAsString() + '\0');
	if(send(sockedFd, msgToSend.c_str(), msgToSend.length(), 0) < 0) {
		throw Common::CommunicationError("Send failed");
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

// std::string TCPClient::Receive(int size)
// {
// 	std::string buf;
// 	buf.resize(size);
// 	if(recv(sockedFd, &buf.front(), size, 0) < 0) {
// 	    throw Common::SocketException("Receive failed!");
//   	}
//   	return buf;
// }

std::string TCPClient::Read()
{
  	char buffer[1] = {'?'};
  	std::stringstream reply;
  	while (buffer[0] != 0) {
		if( recv(sockedFd , buffer , sizeof(buffer) , 0) < 0)
		{
	    	throw Common::SocketException("Receive failed!");
		}
		reply << buffer[0];
	}
	return reply.str();
}

void TCPClient::Close()
{
    close( sockedFd );
}
