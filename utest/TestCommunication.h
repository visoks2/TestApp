#include <chrono>
#include <gtest/gtest.h>
#include <iostream>
#include <thread>

#include "Common.h"
#include "Exceptions.h"
#include "MessagesHandler.h"
#include "TCPClient.h"
#include "TCPServer.h"

#include <fstream>

namespace
{
class TestCommunication : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // clean up db file
        std::remove(DB_STORAGE_FILE_NAME);
    }

    void TearDown() override
    {
    }

    int32_t getDbFileSize()
    {
        std::ifstream in(DB_STORAGE_FILE_NAME, std::ifstream::ate | std::ifstream::binary);
        DEBUG_LOG(in.tellg());
        return in.tellg();
    }
};

TEST_F(TestCommunication, StartServerDontThrowExceptions)
{
    Server::TCPServer server{};
    ASSERT_NO_THROW(server.AcceptNewClients());
}

TEST_F(TestCommunication, ClientThrowsIfServerOffline)
{
    ASSERT_THROW(Client::TCPClient(DEFAULT_IP, DEFAULT_PORT), Common::SocketException);
}

TEST_F(TestCommunication, ServerClosesOnDestructor)
{
    {
        Server::TCPServer server{};
        ASSERT_TRUE(server.is_online());
        ASSERT_NO_THROW(server.AcceptNewClients());
        Client::TCPClient client(DEFAULT_IP, DEFAULT_PORT);
        client.Authenticate(MASTER_PASSWORD);
        client.Create("id_client", "client", "passclient");
    } // Destroys server object when leaves this scope

    // expect to throw, because server is offline
    ASSERT_THROW(Client::TCPClient(DEFAULT_IP, DEFAULT_PORT), Common::SocketException);
}

TEST_F(TestCommunication, CommunicationWithMultipleClients)
{
    ASSERT_EQ(getDbFileSize(), -1); // -1 since there is no file

    for (size_t i = 0; i < 10; i++)
    {
        Server::TCPServer server{};
        server.AcceptNewClients();
        Client::TCPClient client(DEFAULT_IP, DEFAULT_PORT);
        Client::TCPClient client2(DEFAULT_IP, DEFAULT_PORT);
        client.Authenticate(MASTER_PASSWORD);
        client2.Authenticate(MASTER_PASSWORD);
        client.Create(std::string{"id_client"} + std::to_string(i), "client", "passclient");
        client2.Create(std::string{"id_client2"} + std::to_string(i), "client2", "passclient2");
    }
    ASSERT_GT(getDbFileSize(), 0); // db file not empty
}

TEST_F(TestCommunication, AuthenticateThrowsWithWrongPsw)
{
    Server::TCPServer server{};
    server.AcceptNewClients();
    Client::TCPClient client(DEFAULT_IP, DEFAULT_PORT);
    ASSERT_THROW(client.Authenticate("12345678"), Common::AuthenticationFailedException);
    ASSERT_THROW(client.Authenticate("UNKNOWN"), Common::AuthenticationFailedException);
    ASSERT_THROW(client.Authenticate("password"), Common::AuthenticationFailedException);
    ASSERT_THROW(client.Authenticate("admin"), Common::AuthenticationFailedException);
    ASSERT_THROW(client.Authenticate("root"), Common::AuthenticationFailedException);
}

TEST_F(TestCommunication, CannotChangeOrReadDBIfNotAuthenticated)
{
    Server::TCPServer server{};
    server.AcceptNewClients();
    Client::TCPClient client(DEFAULT_IP, DEFAULT_PORT);
    ASSERT_THROW(client.Authenticate("12345678"), Common::AuthenticationFailedException);
    ASSERT_THROW(client.Create("id", "name", "pass"), Common::AuthenticationFailedException);
    // TODO: FIXME
    // ASSERT_THROW(client.Read(), Common::AuthenticationFailedException);
    // ASSERT_THROW(client.Update("id", "name", "pass"), Common::AuthenticationFailedException);
    ASSERT_THROW(client.Delete("id", "name"), Common::AuthenticationFailedException);
}
} // namespace
