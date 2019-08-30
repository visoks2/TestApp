#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fcntl.h>
#include "Common.h"
#include "Exceptions.h"

#include "dataStorageHandler.h"
#include <data.pb.h>

namespace Server {
    class MessagesHandler
    {
    public:
        MessagesHandler()
        {
            
        };

        ~MessagesHandler() 
        {
            
        };

        std::string ProcessMessage(socket_description& user, std::string& aMsg) 
        {
            gpb::Message parsedMsg;
            if(!parsedMsg.ParseFromString(aMsg)) {
                // TODO: throw
            }
            if ((!user.isAuthenticated) && (parsedMsg.action() != gpb::Message_Action::Message_Action_AUTHENTICATE)) {
                throw Common::AuthenticationFailedException();
            }

            switch (parsedMsg.action())
            {
                case gpb::Message_Action::Message_Action_CREATE: {
                    storageHandler.Store(parsedMsg.record());
                    return std::string("created"); // TODO: send normal response
                }
                case gpb::Message_Action::Message_Action_READ: {
                    return storageHandler.Read(parsedMsg.record());
                }
 
                case gpb::Message_Action::Message_Action_UPDATE: {
                    storageHandler.Update(parsedMsg.record());
                    return std::string("update"); // TODO: send normal response
                }
                case gpb::Message_Action::Message_Action_DELETE: {
                    storageHandler.Remove(parsedMsg.record());
                    return std::string("deleted"); // TODO: send normal response
                }
                case gpb::Message_Action::Message_Action_AUTHENTICATE: {
                    user.isAuthenticated = storageHandler.Authenticate(parsedMsg.record());
                    return user.isAuthenticated ? std::string("come on in") : std::string("You not shall pass"); // TODO: send normal response
                }
                default:
                    break;
            }

            return std::string("unknown command");
        }
    private:
        dataStorageHandler storageHandler;
    };
    
}
