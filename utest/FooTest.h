#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "TCPClient.h"
#include "TCPServer.h"

#define LINE_ENTRY std::cout << __LINE__ << std::endl;

constexpr int 				SERVER_PORT			(5050);
const	  std::vector<int> 	SOCKET_OPTIONS 		{ SO_REUSEPORT, SO_REUSEADDR };

TEST(FooTest, t1) { 
    Server::TCPServer server(SERVER_PORT, SOCKET_OPTIONS);
    ASSERT_TRUE(server.is_online());
    ASSERT_NO_THROW(server.AcceptNewClients());
    ASSERT_NO_THROW(server.Receive());
    ASSERT_NO_THROW(server.~TCPServer());
}
 
TEST(FooTest, t2) { 
    Server::TCPServer server(SERVER_PORT, SOCKET_OPTIONS);
    ASSERT_TRUE(server.is_online());

    server.AcceptNewClients();
    server.Receive();
    ASSERT_TRUE(server.is_online());
    {
        Client::TCPClient client("127.0.0.1", 5050);
        client.Send("labas");
        std::cout << client.Receive() << std::endl;
    }
    ASSERT_TRUE(server.is_online());
        Client::TCPClient client("127.0.0.1", 5050);
        client.Send("sup!");
        std::cout << client.Receive() << std::endl;

    ASSERT_TRUE(server.is_online());
    

}
TEST(FooTest, t3) { 
    Server::TCPServer server(5050, {});
    server.AcceptNewClients();
    server.Receive();
    ASSERT_ANY_THROW(Client::TCPClient client("DEADBEEF", 5050));
    ASSERT_TRUE(server.is_online());
}
 