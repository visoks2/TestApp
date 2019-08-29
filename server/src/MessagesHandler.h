#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <fcntl.h>
#include "Common.h"

#include <google/protobuf/util/delimited_message_util.h>

#include <data.pb.h>
namespace Server {
    class MessagesHandler
    {
    public:
        MessagesHandler()
        {
            std::string line;
            std::ifstream myfile (FILE);
            if (myfile.is_open())
            {
                gpb::Message_Record msg;
                bool clean_eof;
                google::protobuf::io::IstreamInputStream is{&myfile};
                while (google::protobuf::util::ParseDelimitedFromZeroCopyStream(&msg, &is, &clean_eof))
                {
                    records.push_back(msg);
                }
                myfile.close();
            }
        };

        ~MessagesHandler() 
        {
            std::ofstream newFile(FILE);

            if(newFile.is_open()) {
                for (auto &&rec : records) {
                    google::protobuf::util::SerializeDelimitedToOstream(rec, &newFile);
                }
            }
            else {
                // You're in trouble now Mr!
            }
            newFile.close();

        };

        std::string ProcessMessage(std::string& aMsg) 
        {
            if (aMsg.empty()) return {};
            gpb::Message parsedMsg;
            if(!parsedMsg.ParseFromString(aMsg)) {
                // TODO: throw
            }
            switch (parsedMsg.action())
            {
                case gpb::Message_Action::Message_Action_CREATE: {
                    records.push_back(parsedMsg.record());
                    return std::string("created");
                }
                case gpb::Message_Action::Message_Action_DELETE: {
                    std::string idToDelete      ( parsedMsg.record().id()       );
                    std::string userToDelete    ( parsedMsg.record().username() );
                    auto rec = std::find_if(records.begin(), records.end(), [idToDelete, userToDelete](gpb::Message_Record& aRec) {
                                                                            return aRec.id()        == idToDelete 
                                                                                && aRec.username()  == userToDelete;
                                                                        });
                    records.erase(rec);
                    return std::string("deleted");
                }
                default:
                    break;
            }

            return std::string("unknown command");
        }
    private:
        const char * FILE = "db.txt";
        std::vector<gpb::Message_Record> records;
    };
    
}
