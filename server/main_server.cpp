
#include <csignal>
#include <ctime>
#include <fstream>
#include <iostream>
#include <thread>

#include "Exceptions.h"
#include "MessagesHandler.h"
#include "TCPServer.h"
#include "socket_description.h"

using namespace Server;

constexpr int SERVER_PORT(5050);
const std::vector<int> SOCKET_OPTIONS{SO_REUSEPORT, SO_REUSEADDR};

bool running = true;
void close_app(int s)
{
    running = false;
}

void writeToLog(std::string &msgToLog)
{
    std::ofstream outfile;
    outfile.open("test.txt", std::ios_base::app);
    outfile << msgToLog << std::endl;
}

int main(int /* argc  */, char ** /* argv */)
{
    std::signal(SIGINT, close_app);
    std::signal(SIGKILL, close_app);

    try
    {
        TCPServer tcp;
        tcp.AcceptNewClients();
        while (running)
        {
            /* code */
        }
    }
    catch (const Common::SocketException &e)
    {
        std::cout << e.what() << '\n';
    }
    std::cout << "Closing" << std::endl;
    return 0;
}