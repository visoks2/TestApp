#include "TCPServer.h"
#include "Exceptions.h"
#include <sstream>

using namespace Server;

bool TCPServer::isOnline;

TCPServer::TCPServer() : keepRunning(true)
{
    isOnline = false;
    int opt = 1;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    memset(&serverAddress, 0, sizeof(serverAddress));

    for (unsigned int i = 0; i < SOCKET_OPTIONS.size(); i++)
    {
        if ((setsockopt(sockfd, SOL_SOCKET, SOCKET_OPTIONS.size(), &opt, sizeof(opt))) < 0)
        {
            throw Common::SocketException("setsockopt failed");
        }
    }
    setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&SELECT_TIMEOUT, sizeof(SELECT_TIMEOUT));

    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(SERVER_PORT);

    if ((bind(sockfd, reinterpret_cast<struct sockaddr *>(&serverAddress), sizeof(serverAddress))) < 0)
    {
        throw Common::SocketException("bind failed");
    }

    if (listen(sockfd, 5) < 0)
    {
        throw Common::SocketException("listen failed");
    }
    isOnline = true;
}

void TCPServer::listenLoop(TCPServer *aServer, const int id)
{
    auto &desc = aServer->newsockfd.at(id);
    DEBUG_LOG_MSG(desc, std::string("New client!"));
    while (aServer->keepRunning)
    {
        std::stringstream ss;
        char buffer[1] = {'?'};
        while (buffer[0] != 0)
        {
            ssize_t receivedSize = recv(desc.socket, buffer, sizeof(buffer), 0);
            if (receivedSize != -1)
            {
                if (receivedSize == 0)
                {
                    DEBUG_LOG_MSG(desc, std::string("Client left"));
                    aServer->isOnline = false;
                    close(desc.socket);
                    aServer->newsockfd.erase(id);
                    return;
                }
                ss << buffer[0];
            }
        }
        try
        {
            std::string str(ss.str());
            str.erase(str.length() - 1);
            std::string response = aServer->messagesHandler.ProcessMessage(desc, str) + '\0';
            send(desc.socket, response.c_str(), response.length(), 0);
        }
        catch (const Common::AuthenticationFailedException &)
        {
            // nope, this client must be forced to leave
            close(desc.socket);
            aServer->newsockfd.erase(id);
        }
        catch (const std::exception &e)
        {
            // everythings ok, nothing to see here
            std::cerr << e.what() << '\n';
        }
        catch (...)
        {
            // hope it never gets to this point
            close(desc.socket);
            aServer->newsockfd.erase(id);
        }

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

    for (auto &&t : threads)
    {
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

void TCPServer::welcomeLoop(TCPServer *aServer)
{
    while (aServer->isAcceptingNewClients)
    {
        socklen_t sosize = sizeof(clientAddress);
        int fd = accept(aServer->sockfd, (struct sockaddr *)&aServer->clientAddress, &sosize);
        if (fd < 0)
            continue;

        static int clientID(0);
        clientID++;

        // hurray, got client, accept it
        socket_description so(fd, inet_ntoa(aServer->clientAddress.sin_addr), clientID);
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
