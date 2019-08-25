
#include <iostream>
#include <csignal>
#include <ctime>
#include <thread>
#include "TCPServer.h"
#include "exceptions.h"

TCPServer tcp;
int num_message = 0;
int time_send   = 1;

void close_app(int s) {
	tcp.Close();
	exit(0);
}

void send_client(descript_socket* desc) {

	while(1) {
		if(!tcp.is_online() && tcp.get_last_closed_sockets() == desc->id) {
			std::cout << "Client disconnected ( id:" << desc->id << " ip:" << desc->ip << " )" << std::endl;
			break;
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
		tcp.Send(date, desc->id);
		sleep(time_send);
	}

}

void received()
{
	std::vector<struct descript_socket*> desc;
	while(1)
	{
		desc = tcp.getMessage();
		for(unsigned int i = 0; i < desc.size(); i++) {
			if( desc[i]->message != "" )
			{
				if(!desc[i]->enable_message_runtime) 
				{
					desc[i]->enable_message_runtime = true;
			        std::thread t(send_client, desc[i]);
					t.detach();
					num_message++;
				}
				std::cout << "id:      " << desc[i]->id      << std::endl
				     << "ip:      " << desc[i]->ip      << std::endl
				     << "message: " << desc[i]->message << std::endl
				     << "socket:  " << desc[i]->socket  << std::endl
				     << "enable:  " << desc[i]->enable_message_runtime << std::endl;
				tcp.clean(i);
			}
		}
		usleep(1000);
	}
}

int main(int argc, char **argv)
{
	if(argc < 2) {
		std::cout << "Usage: ./server port (opt)time-send" << std::endl;
		return 0;
	}
	if(argc == 3)
		time_send = atoi(argv[2]);

	std::signal(SIGINT, close_app);
	std::signal(SIGKILL, close_app); 

    std::vector<int> opts = { SO_REUSEPORT, SO_REUSEADDR };
	try
	{
		int port = std::atoi(argv[1]);
		tcp.Setup(port,opts);
		std::thread thread_accept(received);
		thread_accept.detach();
		while(1) {
			tcp.AcceptNewClients();
		}
	}
	catch(const SocketException& e)
	{
		std::cout << e.what() << '\n';
	}
	return 0;
}