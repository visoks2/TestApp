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

#include <data.pb.h>

namespace Client {

  class TCPClient
  {
    public:
      TCPClient(std::string address , int port);
      ~TCPClient();
      void Send(gpb::Message & message);
      void Create(std::string&& aId, std::string&& aName, std::string&& aPsw);
      void Delete(std::string&& aId, std::string&& aName);
      std::string Receive(int size = 1024);
      std::string Read();
      void Close();
    private:
      int sock;
      std::string address;
      int port;
      struct sockaddr_in server;
  };

}
