#pragma once

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netdb.h> 
#include <vector>
#include <sstream>


class TCPClient
{
  public:
    TCPClient();
    ~TCPClient();
    void Setup(std::string address, int port);
    void Send(std::string data);
    std::string Receive(int size = 1024);
    std::string Read();
    void Close();
  private:
    int sock;
    std::string address;
    int port;
    struct sockaddr_in server;
};

