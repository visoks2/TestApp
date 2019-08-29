#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <thread>

#include "TCPClient.h"
#include "TCPServer.h"
#include "MessagesHandler.h"
#include "Common.h"


// TEST(FooTest, t1) {
//     Server::TCPServer server(Server::MessagesHandler{});
//     ASSERT_TRUE(server.is_online());
//     ASSERT_NO_THROW(server.AcceptNewClients());
//     ASSERT_NO_THROW(server.Receive());
//     Client::TCPClient client("127.0.0.1", 5050);
//     client.Send("???");
//     std::cout << client.Receive() << std::endl;
//     std::this_thread::sleep_for(std::chrono::seconds(1));
// }

// TEST(FooTest, t2) {
//     Server::TCPServer server(Server::MessagesHandler{});
//     ASSERT_TRUE(server.is_online());

//     server.AcceptNewClients();
//     server.Receive();
//     ASSERT_TRUE(server.is_online());
//     {
//         Client::TCPClient client("127.0.0.1", 5050);
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//         client.Send("labas");
//         client.Send("labas");
// //         std::cout << client.Receive() << std::endl;
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     }
//     ASSERT_TRUE(server.is_online());
//     std::this_thread::sleep_for(std::chrono::seconds(1));
//         Client::TCPClient client("127.0.0.1", 5050);
//         client.Send("sup!");
// //         std::cout << client.Receive() << std::endl;

//     std::this_thread::sleep_for(std::chrono::seconds(1));
//     ASSERT_TRUE(server.is_online());


// }

 TEST(FooTest, tasd1) {
    Server::TCPServer server {};
    ASSERT_TRUE(server.is_online());
    ASSERT_NO_THROW(server.AcceptNewClients());
    Client::TCPClient client("127.0.0.1", 5050);
    Client::TCPClient client2("127.0.0.1", 5050);
    client.Authenticate(MASTER_PASSWORD);
    client.Create("id_client", "client", "passclient");
    client2.Authenticate(MASTER_PASSWORD);
    {
        Client::TCPClient client3("127.0.0.1", 5050);
        client3.Authenticate(MASTER_PASSWORD);
        client.Create("id2", "asas", "pass");
        client2.Create("id_client22", "asdasda", "passclient2");
        client3.Create("id33client3", "name2", "pass2client3");
        client3.Create("id33client3", "client3client3", "pass2");
    }
    client.Create("id33", "name", "pass2");
    client2.Delete("id33", "name");
    DEBUG_LOG(client.Read());
    DEBUG_LOG(client.Read());
    DEBUG_LOG(client.Read());

    std::this_thread::sleep_for(std::chrono::seconds(1));
}





