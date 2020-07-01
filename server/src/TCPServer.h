#pragma once

#include <algorithm>
#include <arpa/inet.h>
#include <atomic>
#include <cctype>
#include <iostream>
#include <map>
#include <memory>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <unistd.h>
#include <vector>

#include "MessagesHandler.h"
#include "socket_description.h"

namespace Server
{
constexpr int MAXPACKETSIZE(1024);
constexpr int SERVER_PORT(5050);
const std::vector<int> SOCKET_OPTIONS{SO_REUSEPORT, SO_REUSEADDR};
constexpr int REFRESH_INTERVAL{100};
constexpr int SEND_INTERVAL{100};

class TCPServer
{

  public:
    TCPServer();
    ~TCPServer();

    void AcceptNewClients();
    static bool is_online();

    std::vector<std::thread> threads;
    std::atomic<bool> keepRunning;

  private:
    static void welcomeLoop(TCPServer *aServer);
    std::thread acceptNewClients;
    bool isAcceptingNewClients;

    std::thread receiveThread;

    int sockfd;
    struct sockaddr_in serverAddress;
    struct sockaddr_in clientAddress;

    std::map<int, socket_description> newsockfd;

    static bool isOnline;
    static void listenLoop(TCPServer *aServer, const int id);
    MessagesHandler messagesHandler;
};
} // namespace Server
