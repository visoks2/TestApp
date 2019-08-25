#include <iostream>
#include <csignal>
#include "TCPClient.h"
#include "exceptions.h"

TCPClient tcp;
bool isClosing = false;
void close_app(int s)
{
	isClosing = true;
	tcp.Close();
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
		tcp.Setup(argv[1], std::atoi(argv[2]));
		while(!isClosing)
		{
			tcp.Send(argv[3]);
			std::string rec = tcp.Read();
			if( rec != "" )
			{
				static int count (0);
				count++;
				std::cout << count << "\t" << rec << std::endl;
			}
			sleep(1);
		}
	}
	catch(const SocketException& e)
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
