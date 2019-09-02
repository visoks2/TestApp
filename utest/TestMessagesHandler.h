#include <gtest/gtest.h>
#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <memory>

#include "Exceptions.h"
#include "TCPClient.h"
#include "TCPServer.h"
#include "MessagesHandler.h"
#include "Common.h"

#include "MockdataStorageHandler.h"
#include "socket_description.h"
#include <data.pb.h>

using ::testing::Return;
using ::testing::_; // Matcher for parameters


namespace {
    class TestMessagesHandler : public ::testing::Test {
    protected:
        void SetUp() override {
        }

        void TearDown() override {
        }

        Server::socket_description getSocket(){
            return Server::socket_description (1, "test", 0);
        }    

        gpb::Message getMessage(gpb::Message_Action aAction){
            gpb::Message message;
            message.set_action(aAction);
            auto* record = message.mutable_record();
            record->set_id("test");
            record->set_username("test");
            record->set_password("test");
            return message;
        }    


    };
    TEST_F(TestMessagesHandler, ThrowOnInvalidInput) {
        std::shared_ptr<MockdataStorageHandler> mockStorage = std::shared_ptr<MockdataStorageHandler>();
        EXPECT_CALL(*mockStorage, Store(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Read(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Update(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Remove(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Authenticate(_))
            .Times(0);

        Server::MessagesHandler messageHandler(mockStorage);
        Server::socket_description socket = getSocket();
        std::string msg{"INVALID MESSAGE!"};
        ASSERT_THROW(messageHandler.ProcessMessage(socket, msg), Common::MessageParseException);
    }

    TEST_F(TestMessagesHandler, AuthenticateMessageProcessedCorrectly) {
        std::shared_ptr<MockdataStorageHandler> mockStorage = std::shared_ptr<MockdataStorageHandler>();
        EXPECT_CALL(*mockStorage, Store(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Read(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Update(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Remove(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Authenticate(_))
            .Times(1).WillOnce(Return(true));

        Server::MessagesHandler messageHandler(std::move(mockStorage));
        Server::socket_description socket = getSocket();
        gpb::Message message;
        message.set_action(gpb::Message_Action_AUTHENTICATE);
        auto* record = message.mutable_record();
        record->set_password("MASTER_PASSWORD");

        std::string msg { message.SerializeAsString() };


        messageHandler.ProcessMessage(socket, msg);
    }

    TEST_F(TestMessagesHandler, StoreMessageProcessedCorrectly) {
        std::shared_ptr<MockdataStorageHandler> mockStorage = std::shared_ptr<MockdataStorageHandler>();
        EXPECT_CALL(*mockStorage, Store(_))
            .Times(1);
        EXPECT_CALL(*mockStorage, Read(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Update(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Remove(_))
            .Times(0);
        EXPECT_CALL(*mockStorage, Authenticate(_))
            .Times(0);

        Server::MessagesHandler messageHandler(std::move(mockStorage));
        Server::socket_description socket = getSocket();
        gpb::Message message = getMessage(gpb::Message_Action_CREATE);
        std::string msg { message.SerializeAsString() };


        messageHandler.ProcessMessage(socket, msg);
    }
}

