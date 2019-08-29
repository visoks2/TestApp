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
  /**	
   * Class that connects to tcp socket and send request to create, read, update or delete credentials data
   */
  class TCPClient
  {
    public:
      /**	
       * @brief 	Creates TCP client
       * 
       * @param	  aAddress	Server address
       * @param	  aPort		Server port
       * 
       * @throws	Common::SocketException		Cound not create socket or connect to server
       * 
       */
      TCPClient(std::string aAddress , int aPort);

      /**	
       * @brief 	Closes TCP client
       */
      ~TCPClient();

      /**	
       * @brief 	Creates credentials data
       * 
       * @param	  aId	  Credential id
       * @param	  aName	Username
       * @param	  aPsw	Password
       * 
       * @throws	Common::SocketException		  Socket is not opened
       * @throws	Common::CommunicationError  Error happened during send operation
       * 
       */
      void Create(std::string&& aId, std::string&& aName, std::string&& aPsw);

      /**	
       * @brief 	Deletes credentials data
       * 
       * @param	  aId	  Credential id
       * @param	  aName	Username
       * 
       * @throws	Common::SocketException		  Socket is not opened
       * @throws	Common::CommunicationError  Error happened during send operation
       * 
       */
      void Delete(std::string&& aId, std::string&& aName);

      
      // std::string Receive(int size = 1024);
      std::string Read();

    private:
      /**	
       * @brief 	Closes TCP socket
       */
      void Close();

      /**	
       * @brief 	Sends message to server
       * 
       * @param	  aMessage	Reference to message
       * 
       * @throws	Common::SocketException		  Socket is not opened
       * @throws	Common::CommunicationError  Error happened during send operation
       * 
       */
      void Send(gpb::Message & aMessage);


      /**	
       * @brief 	Socket descriptor
       */
      int sockedFd;

      /**	
       * @brief 	Structure describing an Internet socket address.
       */
      struct sockaddr_in server;
  };

}
