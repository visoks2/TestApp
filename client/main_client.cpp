#include <iostream>
#include <csignal>

#include "TCPClient.h"
#include "Exceptions.h"

using namespace Client;

bool running = true;
void close_app(int s)
{
	running = false;
}

int main(int argc, char *argv[])
{
	if(argc != 4) {
		std::cout << "Usage: ./client ip port" << std::endl;
		return 0;
	}
	std::signal(SIGINT, close_app);
	std::signal(SIGKILL, close_app); 

	try
	{
		std::string masterPsw;
		TCPClient tcp(argv[1], std::atoi(argv[2]));
		std::cout << "Connected!" << std::endl;
		std::cout << "Enter master password:" << std::endl;
		std::cin >> masterPsw;
		std::cout << masterPsw << std::endl;
		tcp.Authenticate(masterPsw);
		// while(running)
		// {
		// 	tcp.Send(argv[3]);
		// 	std::string rec = tcp.Receive();
		// 	if( rec != "" ) {
		// 		std::cout << rec << std::endl;
		// 	}
		// 	usleep(100);
		// }
	}
	catch(const Common::SocketException& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch(const std::exception& e)
	{
		std::cout << e.what() << std::endl;
		return 1;
	}
	catch(...)
	{
		std::cout << "something really bad happened ¯\\_(ツ)_/¯" << std::endl;
		return 1;
	}
	return 0;
}
